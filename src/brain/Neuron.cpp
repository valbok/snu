/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @date Apr 2015
 * @copyright VaL Doroshchuk
 * @license GNU GPL v2
 * @package Snu
 */

#include "Neuron.h"
#include <cmath>
#include <cstdlib>
#include <time.h>
#include <ctime>

/// Threshold of membrane potential to make a spike.
const float kSpikeThreshold = 35.0f;

/// Default value for membrane potential.
const float kDefaultMembrane = -60.0f;

const float kMembraneAfterSpike = -40.0f;

Connection::Connection(Neuron* target, float weight, float max, float expire)
    : m_target(target)
    , m_weight(weight)
    , m_max(max)
    , m_expire(expire)
{
}

float Connection::weight() const
{
    int sign = m_weight > 0 ? 1 : -1;
    float w = abs(m_weight);
    float v = m_max * (1 - exp(-w / m_expire));

    return sign * v;
}

void Connection::spike()
{
    m_target->apply(weight(), this);
}

void Connection::teach(float value)
{
    if (m_weight < 0)
        m_weight -= value;
    else
        m_weight += value;
}

Neuron::Neuron(float extI, float expireSpeedFired, float expireSpeedNotFired)
    : m_extI(extI)
    , m_expireSpeedFired(expireSpeedFired)
    , m_expireSpeedNotFired(expireSpeedNotFired)
    , m_prevMem(kDefaultMembrane)
    , m_curMem(kDefaultMembrane)
{
}

void Neuron::connectTo(Neuron* target, float weight)
{
    m_connections.push_back({target, weight});
}

float Neuron::prevMem() const
{
    // Some magic configurable params of Izhikevich neuron model.
    const float k = 0.5f;
    const float vt = -45.0f;
    // Capacitance of neuron, dimension pF.
    const float cm = 50.0f;

    return (k * (m_prevMem - kDefaultMembrane) * (m_prevMem - vt) -
        m_prevAdd + m_extI + m_synI) / cm;
}

float Neuron::prevAdd() const
{
    // Some magic configurable params of Izhikevich neuron model.
    const float a = 0.02f;
    const float b = 0.5f;

    return a * (b * (m_prevMem - kDefaultMembrane) - m_prevAdd);
}

bool Neuron::tick(float h)
{
    m_fired = m_prevMem > kSpikeThreshold;
    if (m_prevMem > kMembraneAfterSpike)
        m_refractoryPeriod = true;

    if (m_fired) {
        m_firedPeriod = true;
        m_curMem = kMembraneAfterSpike;
        m_curAdd = m_prevAdd + 100.0f;
        m_synI *= m_expireSpeedFired ? exp(-h / m_expireSpeedFired) : 0;

        for (size_t i = 0; i < m_connections.size(); ++i)
            m_connections[i].spike();

        for (size_t i = 0; i < m_incomingSpikes.size(); ++i)
            m_incomingSpikes[i]->teach(1);

        m_incomingSpikes.clear();
    } else {
        m_curMem = m_prevMem + h * prevMem();
        m_curAdd = m_prevAdd + h * prevAdd();
        m_synI *= m_expireSpeedNotFired ? exp(-h / m_expireSpeedNotFired) : 1;

        // All prev spikes could not activate current neuron.
        // TODO: val should be param?
        const float activateThreshold = -57.0f;
        if (m_prevMem > activateThreshold && m_curMem <= activateThreshold) {
            for (size_t i = 0; i < m_incomingSpikes.size(); ++i)
                m_incomingSpikes[i]->teach(-1);

            m_incomingSpikes.clear();
        }

        if (m_prevMem <= kDefaultMembrane)
            m_firedPeriod = false;

        if (!m_firedPeriod && m_prevMem > kDefaultMembrane)
            m_refractoryPeriod = false;
    }

    m_prevMem = m_curMem;
    m_prevAdd = m_curAdd;

    return m_fired;
}

void Neuron::spike()
{
    m_prevMem = kSpikeThreshold + 0.1f;
}

bool Neuron::apply(float value, Connection* synapse)
{
    if (m_refractoryPeriod)
        return false;

    m_synI += value;
    if (synapse)
        m_incomingSpikes.push_back(synapse);

    return true;
}

const std::vector<Connection*>& Neuron::incomingSpikes() const
{
    return m_incomingSpikes;
}

float Neuron::syn() const
{
    return m_synI;
}

float Neuron::mem() const
{
    return m_prevMem;
}

bool Neuron::fired() const
{
    return m_fired;
}

const Connections& Neuron::connections() const
{
    return m_connections;
}

