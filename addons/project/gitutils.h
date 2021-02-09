/*
    SPDX-FileCopyrightText: 2021 Waqar Ahmed <waqar.17a@gmail.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/
#ifndef GITUTILS_H
#define GITUTILS_H

#include <QString>

namespace GitUtils
{
enum RefType {
    Head = 0x1,
    Remote = 0x2,
    Tag = 0x4,
    All = 0x7,
};

/**
 * @brief Represents a branch
 */
struct Branch {
    /** Branch name */
    QString name;
    /** remote name, will be empty for local branches */
    QString remote;
    /** Ref type @see RefType */
    RefType type;
};

struct CheckoutResult {
    QString branch;
    QString error;
    int returnCode;
};

bool isGitRepo(const QString &repo);

QString getCurrentBranchName(const QString &repo);

CheckoutResult checkoutBranch(const QString &repo, const QString &branch);

CheckoutResult checkoutNewBranch(const QString &repo, const QString &newBranch, const QString &fromBranch = QString());

/**
 * @brief get all local and remote branches
 */
QVector<Branch> getAllBranches(const QString &repo);
/**
 * @brief get all local and remote branches + tags
 */
QVector<Branch> getAllBranchesAndTags(const QString &repo, RefType ref = RefType::All);
}

#endif // GITUTILS_H