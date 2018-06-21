#ifndef TETHER_H
#define TETHER_H

#include <memory>
#include <functional>
#include <map>
#include <boost/signals2/signal.hpp>
#include <boost/bind/bind.hpp>
#include <boost/optional/optional.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>


template<class StateType>
using TetherSig = boost::signals2::signal<void (const StateType& state)>;

template<class StateType>
using CircuitSig = boost::signals2::signal<StateType (const std::weak_ptr<StateType>& shp)>;


template<class StateType>
class Tether
{

public:
    explicit Tether(const StateType& arg);
    template<class... StateArgs>
    explicit Tether(const StateArgs&... args);

    virtual ~Tether();
    Tether(const Tether&) = delete;
    Tether(Tether&&) = delete;

    virtual boost::uuids::uuid make_tether(TetherSig<StateType>& signal);
    virtual void cut_tether(const boost::uuids::uuid& id);
    virtual void resolve_tether();

    template<class Hooker>
    boost::uuids::uuid make_hooker(const Hooker& hooker);
    virtual void cut_hooker(const boost::uuids::uuid& id);
    virtual void resolve_hooker();

    virtual const StateType get_state();

private:
    virtual void set_state_slot(const StateType& state);

private:
    CircuitSig<StateType> state_changed;
    std::shared_ptr<StateType> state_ptr;
    std::map<boost::uuids::uuid, boost::signals2::connection> tether_con_map;
    std::map<boost::uuids::uuid, boost::signals2::connection> circuit_con_map;
};


template<class StateType>
Tether<StateType>::Tether(const StateType& arg)
{
    state_ptr = std::make_shared<StateType>(arg);
}


template<class StateType>
template<class... StateArgs>
Tether<StateType>::Tether(const StateArgs&... args)
{
    state_ptr = std::make_shared<StateType>(args...);
}


template<class StateType>
Tether<StateType>::~Tether()
{
    state_ptr.reset();
    tether_con_map.clear();
    circuit_con_map.clear();
}


template<class StateType>
boost::uuids::uuid Tether<StateType>::make_tether(TetherSig<StateType>& signal)
{
    auto tether_con = signal.connect(boost::bind(&Tether::set_state_slot, this, _1));
    auto id = boost::uuids::random_generator()();
    tether_con_map.insert(std::make_pair(id, tether_con));
    return id;
}


template<class StateType>
void Tether<StateType>::cut_tether(const boost::uuids::uuid &id)
{
    tether_con_map[id].disconnect();
    tether_con_map.erase(id);
}


template<class StateType>
void Tether<StateType>::resolve_tether()
{
    for(auto& tether_con : tether_con_map){
        tether_con.second.disconnect();
    }

    tether_con_map.clear();
}


template<class StateType>
template<class Hooker>
boost::uuids::uuid Tether<StateType>::make_hooker(const Hooker& hooker)
{
    auto circuit_con = state_changed.connect(hooker);
    auto id = boost::uuids::random_generator()();
    circuit_con_map.insert(std::make_pair(id, circuit_con));
    return id;
}


template<class StateType>
void Tether<StateType>::cut_hooker(const boost::uuids::uuid &id)
{
    circuit_con_map[id].disconnect();
    circuit_con_map.erase(id);
}


template<class StateType>
void Tether<StateType>::resolve_hooker()
{
    for(auto& circuit_con : circuit_con_map){
        circuit_con.second.disconnect();
    }

    circuit_con_map.clear();
}


template<class StateType>
const StateType Tether<StateType>::get_state()
{
    return *state_ptr;
}


template<class StateType>
void Tether<StateType>::set_state_slot(const StateType& state)
{
    *state_ptr = state;
    std::weak_ptr<StateType> state_weak_ptr = state_ptr;

    if(boost::optional<StateType> result = state_changed(state_weak_ptr)){
        *state_ptr = *result;
    }
}

#endif // TETHER_H
