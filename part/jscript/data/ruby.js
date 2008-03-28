/** kate-script
 * name: Ruby
 * license: LGPL
 * author: Robin Pedersen <robin.pedersen@runbox.com>
 * version: 1
 * kate-version: 3.0
 * type: indentation
 *
 * This file is part of the Kate Project.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License version 2 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

//BEGIN USER CONFIGURATION
//END USER CONFIGURATION


// specifies the characters which should trigger indent, beside the default '\n'
triggerCharacters = "cdefhilnrsuw}]";

// Indent after lines that match this regexp
var rxIndent = /^\s*(def|if|unless|for|while|until|class|module|else|elsif|case|when|begin|rescue|ensure|catch)\b/;

// Unindent lines that match this regexp
var rxUnindent = /^\s*((end|when|else|elsif|rescue|ensure)\b|[\]\}])(.*)$/;


// debug to the term in blue so that it's easier to make out amongst all
// of Kate's other debug output.
function dbg(s)
{
  debug("\u001B[44m" + s + "\u001B[0m");
}

function isComment(attr)
{
  return (attr == 30 || attr == 31);
}

function isCommentAttr(line, column)
{
  var attr = document.attribute(line, column);
  return isComment(attr);
}

// Return the closest non-empty line, ignoring comments
// (result <= line). Return -1 if the document
function findPrevNonCommentLine(line)
{
  line = document.prevNonEmptyLine(line);
  while (line >= 0 && isCommentAttr(line, document.firstColumn(line))) {
    line = document.prevNonEmptyLine(line - 1);
  }
  return line;
}

function isStmtContinuing(line)
{
  // Check for operators at end of line
  var rx = /(\+|\-|\*|\/|\=|&&|\|\||and|or|,|\\)\s*$/;

  return rx.test(document.line(line));
}

// Return the first line that is not preceeded by a "continuing" line.
// Return currLine if currLine <= 0
function findStmtStart(currLine)
{
  var p, l = currLine;
  do {
    if (l <= 0) return l;
    p = l;
    l = findPrevNonCommentLine(l - 1);
  } while (isStmtContinuing(l));
  return p;
}

// Returns a tuple that contains the first and last line of the
// previous statement before line.
function findPrevStmt(line)
{
  var stmtEnd = findPrevNonCommentLine(line);
  var stmtStart = findStmtStart(stmtEnd);

  return {start: stmtStart, end: stmtEnd};
}

function isBlockStart(stmt)
{
  var str = getStmtStr(stmt);

  if (rxIndent.test(str))
    return true;

  var p = str.search(/(do|\{)(\s+\|.*\||\s*)$/);
  if (p != -1 && !isComment(getStmtAttr(stmt, p)))
    return true;

  return false;
}

function isBlockEnd(stmt)
{
  var str = getStmtStr(stmt);

  return rxUnindent.test(str);
}

function findBlockStart(line)
{
  var nested = 0;
  var stmt = {start: line, end: line};
  while (true) {
    if (stmt.start < 0) return stmt;

    stmt = findPrevStmt(stmt.start - 1);
    if (isBlockEnd(stmt)) {
      nested++;
    }
    if (isBlockStart(stmt)) {
      if (nested == 0)
        return stmt;
      else
        nested--;
    }
  }
}

// Return the string representing a possible multi-line statement
// defined by stmt.start and stmt.end
function getStmtStr(stmt)
{
  var str = "";
  for (var l = stmt.start; l <= stmt.end; ++l) {
    str += document.line(l).replace(/\\$/, ' ');
    if (l < stmt.end)
      str += "\n"
  }
  return str;
}

// Return document.attribute at the given offset in a statement
function getStmtAttr(stmt, offset)
{
  var line = stmt.start;
  while (line < stmt.end && document.lineLength(line) < offset) {
    offset -= document.lineLength(line++) + 1;
  }
  return document.attribute(line, offset);
}

// check if the trigger characters are in the right context,
// otherwise running the indenter might be annoying to the user
function isValidTrigger(line, ch)
{
  if (ch == "" || ch == "\n")
    return true; // Explicit align or new line
  if (rxUnindent.test(document.line(line))) {
    if (RegExp.$3 == "")
      return true; // Exact match
  }
  return false;
}

// indent gets three arguments: line, indentwidth in spaces,
// typed character indent
function indent(line, indentWidth, ch)
{
  if (!isValidTrigger(line, ch))
    return -2;

  var prevStmt = findPrevStmt(line - 1);
  dbg("start: " + prevStmt.start + ", end: " + prevStmt.end);

  if (prevStmt.end < 0)
    return -2; // Can't indent the first line

  var prevStmtStr = getStmtStr(prevStmt);
  dbg("foo"); //FIXME Removing this caused testkateregression to crash the same place as when running indent/csmart

  var prevStmtInd = document.firstVirtualColumn(prevStmt.start);

  // Handle indenting of multiline statements.
  // Manually indenting to be able to force spaces.
  if (isStmtContinuing(prevStmt.end) && (prevStmt.end == line-1 || RegExp.$1 != "\\")) {
    var len = document.firstColumn(prevStmt.end);
    var str = document.line(prevStmt.end).substr(0, len);
    if (!document.startsWith(line, str)) {
      document.removeText(line, 0, line, document.firstColumn(line));
      if (prevStmt.start == prevStmt.end)
        str += "    ";
      document.insertText(line, 0, str);
    }
    return -2;
  }

  if (rxUnindent.test(document.line(line))) {
    var startStmt = findBlockStart(line);
    dbg("currLine: " + line);
    dbg("StartLine: {" + startStmt.start + "," + startStmt.end + "}");
    if (startStmt.start >= 0)
      return document.firstVirtualColumn(startStmt.start);
    else
      return -2;
  }

  if (rxIndent.test(prevStmtStr)) {
    return prevStmtInd + indentWidth;
  } else {
    var p = prevStmtStr.search(/(do|\{)(\s+\|.*\||\s*)$/);
    if (p != -1 && !isComment(getStmtAttr(prevStmt, p))) {
      return prevStmtInd + indentWidth;
    } else if (prevStmtStr.search(/[\[\{]\s*$/) != -1) {
      return prevStmtInd + indentWidth;
    }
  }

  // Keep current
  return prevStmtInd;
}

// kate: indent-width 2; indent-spaces on;
