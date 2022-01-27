#pragma once

#include "json.h"

struct ProtocolMessage {
    TaggedMember<"seq", int> messageID;

    /// 'request'
    /// 'response'
    /// 'event'
    /// etc.
    TaggedMember<"type", QString> requestType;
};

struct Request {
    TaggedMember<"seq", int> messageID;

    /// 'request'
    /// 'response'
    /// 'event'
    /// etc.
    TaggedMember<"type", QString> requestType;

    TaggedMember<"command", QString> command;
};
