#ifndef ARG_HPP
#define ARG_HPP
#iclude <vector>

#include "log.hpp"
#include "../rpc/marshaller.hpp"

namespace MemoryBase{
struct RequestVoteArgs
    RequestVoteArgs(){}
    RequestVoteArgs(uint32_t _term, uint32_t _id, uint32_t _pre_index,uint32_t _pre_term)
        : term(_term), id(_id), pre_index(_pre_index), pre_term(_pre_term){}
    
    uint32_t term
    uint32_t id;
    uint32_t last_index;
    uint32_t last_term;
};
struct RequestVoteReply{
    RequestVoteReply(){}
    RequestVoteReply(uint32_t _term, uint32_t _granted)
        : term(_term), granted(_granted){}
    
    uint32_t term;
    uint32_t granted;
};
struct AppendEntryArgs{
    AppendEntryArgs(){}
    AppendEntryArgs(uint32_t _term, uint32_t _id, uint32_t _pre_index, uint32_t _pre_term,
            uint32_t _commit, const std::vector<Log> _logs) : term(_term), id(_id),
            pre_index(_pre_index), pre_term(_pre_term), logs(_logs){}
    
    uint32_t term;
    uint32_t id;
    uint32_t pre_index;
    uint32_t pre_term;
    uint32_t commit;
    std::vector<Log> logs;
}
struct AppendEntryReply{
    AppendEntryReply(){}
    AppendEntryReply(uint32_t _term, uint32_t _match_index)
        : term(_term), match_index(_match_index){};

    uint32_t term;
    uint32_t match_index;
};
inline Marshaller& operator<<(Marshaller &mar, const RequestVoteArgs &args){
    mar << args.term;
    mar << args.id;
    mar << args.last_index;
    mar << args.last_term;
    return mar;
}

inline Marshaller& operator<<(Marshaller &mar, const RequestVoteReply &reply){
    mar << reply.term;
    mar << reply.granted;
    return mar;
}

inline Marshaller& operator<<(Marshaller &mar, const AppendEntryArgs &args){
    mar << args.term;
    mar << args.id;
    mar << args.pre_index;
    mar << args.pre_term;
    mar << args.commit;
    mat << args.logs;
    return mar;
}

inline Marshaller& operator<<(Marshaller &mar, const AppendEntryReply &reply){
    mar << reply.term;
    mar << reply.match_index;
    return mar;
}

inline Marshaller& operator>>(Marshaller &mar, RequestVoteArgs &args){
    mar >> args.term;
    mar >> args.id;
    mar >> args.last_index;
    mar >> args.last_term;
    return mar;
}

inline Marshaller& operator>>(Marshaller &mar, RequestVoteReply &reply){
    mar >> reply.term;
    mar >> reply.granted;
    return mar;
}

inline Marshaller& operator>>(Marshaller &mar, AppendEntryArgs &args){
    mar >> args.term;
    mar >> args.id;
    mar >> args.pre_index;
    mar >> args.pre_term;
    mar >> args.commit;
    mar >> args.logs;
    return mar;
}

inline Marshaller& operator>>(Marshaller &mar, AppendEntryReply &reply){
    mar >> reply.term;
    mar >> reply.match_index;
    return mar;
}

}
#endif
