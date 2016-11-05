#ifndef OCTOPUS_NODE_HPP
#define OCTOPUS_NODE_HPP

namespace octo
{
    class Group;
    class SignalBase;
    
    //! A node is either a SignalBase or a Group
    class Node
    {
    public:
        Node(SignalBase& signalBase) : type(Type::SIGNAL_BASE), signalBase(&signalBase) { }
        Node(Group& group) : type(Type::GROUP), group(&group) { }
        
        bool isSignalBase() const { return type == Type::SIGNAL_BASE; }
        bool isGroup() const { return type == Type::GROUP; }
        
        SignalBase& asSignalBase() { return *signalBase; }
        const SignalBase& asSignalBase() const { return *signalBase; }
        Group& asGroup() { return *group; }
        const Group& asGroup() const { return *group; }

    private:
        enum class Type { SIGNAL_BASE, GROUP } type;
        
        union
        {
            SignalBase* signalBase;
            Group* group;
        };
    };
    
    static inline bool operator==(const Node& lhs, const Node& rhs)
    {
        return (lhs.isSignalBase() && rhs.isSignalBase() && (&lhs.asSignalBase() == &rhs.asSignalBase())) ||
               (lhs.isGroup() && rhs.isGroup() && (&lhs.asGroup() == &rhs.asGroup()));
    }
    
    static inline bool operator!=(const Node& lhs, const Node& rhs)
    {
        return !(lhs == rhs);
    }
}

#endif
