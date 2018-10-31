/*
 * PingStateMachine.hpp
 *
 *  Created on: 31.10.2018
 *      Author: Ingo
 */

#ifndef PINGSTATEMACHINE_HPP_
#define PINGSTATEMACHINE_HPP_


#include <string>
#include <iostream>
#include <boost/msm/front/state_machine_def.hpp>
#include <boost/msm/back/state_machine.hpp>
#include <boost/mpl/vector.hpp>

#include "PingImplementation.h"
#include "PingParameters.h"

namespace msm = boost::msm; // Meta state machine
namespace mpl = boost::mpl; // Meta programming library

// Helper for callbacks
template <class FSM,class EVENT>
struct Callback
{
	Callback(FSM* fsm):fsm_(fsm){}
	void operator()()const
	{
		fsm_->process_event(evt_);
	}
private:
	FSM* fsm_;
	EVENT evt_;
};

// Replace the default no-transition response
template <class FSM,class Event>
void no_transition(Event const& e, FSM&,int state)
{
	std::cout << "no transition from state " << state
			<< " on event " << typeid(e).name() << std::endl;
}

// Events
struct e_init {};
struct e_initSuccess {};
struct e_initFail {};

struct e_setTarget
{
	e_setTarget(string _target)
	: target(_target)
	{}
	string target;
};
struct e_setTTL
{
	e_setTTL( unsigned char _TTL)
	: TTL(_TTL)
	{}
	unsigned char TTL;
};
struct e_setTimeout
{
	e_setTimeout(int _timeout)
	: timeout(_timeout)
	{}
	int timeout;
};
struct e_setCount
{
	e_setCount(int _count)
	: count(_count)
	{}
	int count;
};
struct e_setInterval
{
	e_setInterval(int _interval)
	: interval(_interval)
	{}
	int interval;
};
struct e_setSize
{
	e_setSize(unsigned int _size)
	: size(_size)
	{}
	unsigned int size;
};
struct e_startPing {};
struct e_sendPing{};
struct e_respRcvd{};
struct e_stopPing {};
struct e_uninit{};


struct STM_Ping : public msm::front::state_machine_def<STM_Ping>
{
	STM_Ping(PingImplementation *_pingImpl, PingParameters *_pingParams)
				:pingImpl(_pingImpl), pingParams(_pingParams){}

	template <class Event,class FSM>
	void on_entry(Event const&, FSM&)
	{
		std::cout << "entering: state machine STM_Ping" << std::endl;
	}

	template <class Event,class FSM>
	void on_exit(Event const&, FSM&)
	{
		std::cout << "state machine STM_Ping" << std::endl;
	}

	// The list of FSM states
	struct ST_Uninit : public msm::front::state<>
	{
		template <class Event,class FSM>
		void on_entry(Event const&,FSM& )
		{
			std::cout << "entering: ST_Uninit" << std::endl;
		}
		template <class Event,class FSM>
		void on_exit(Event const&,FSM& )
		{
			std::cout << "leaving: ST_Uninit" << std::endl;
		}
	};

	struct ST_Initializing : public msm::front::state<>
	{
		template <class Event,class FSM>
		void on_entry(Event const&, FSM& fsm)
		{
			std::cout << "entering: ST_Initializing" << std::endl;
			fsm.pingImpl->initialize(fsm.pingParams->ttl, fsm.pingParams->recvTimeout, Callback<FSM, e_initSuccess>(&fsm), Callback<FSM, e_initFail>(&fsm));
		}

		template <class Event,class FSM>
		void on_exit(Event const&,FSM& )
		{
			std::cout << "leaving: ST_Initializing" << std::endl;
		}
	};

	// Sub state machine STM_Initialized
	struct STM_Initialized : public msm::front::state_machine_def<STM_Initialized>
	{
		struct ST_Idle : public msm::front::state<>
		{
			template <class Event,class FSM>
			void on_entry(Event const& ,FSM& fsm)
			{
				std::cout << "entering: ST_Idle" << std::endl;
			}
			template <class Event,class FSM>
			void on_exit(Event const&,FSM& ) {std::cout << "leaving: ST_Idle" << std::endl;}
		};

		struct ST_Pinging : public msm::front::state_machine_def<ST_Pinging>
		{
			template <class Event,class FSM>
			void on_entry(Event const& ,FSM&) {std::cout << "entering: ST_Pinging" << std::endl;}
			template <class Event,class FSM>
			void on_exit(Event const&,FSM& ) {std::cout << "leaving: ST_Pinging" << std::endl;}

			struct ST_WaitForResponse : public msm::front::state<>
			{
				template <class Event,class FSM>
				void on_entry(Event const& ,FSM&) {std::cout << "entering: ST_WaitForResponse" << std::endl;}
				template <class Event,class FSM>
				void on_exit(Event const&,FSM& ) {std::cout << "leaving: ST_WaitForResponse" << std::endl;}
			};

			struct ST_PingActive : public msm::front::state<>
			{
				template <class Event,class FSM>
				void on_entry(Event const& ,FSM&)
				{
					cout << "entering: ST_PingActive" << endl;
				}
				template <class Event,class FSM>
				void on_exit(Event const&,FSM&)
				{
					cout << "leaving: ST_PingActive" << endl;
				}

				PingImplementation* pingImpl;

			}; // ST_PingActive

			// Make all orthogonal regions of ST_Pinging initial states
			typedef mpl::vector<ST_WaitForResponse, ST_PingActive> initial_state;

			// Transition actions
			void a_sendPing(e_sendPing const& evt)
			{
				std::cout << "a_sendPing() called\n";
				//evt.ping->sendPing();
			}

			void a_processResponse(e_respRcvd const& evt)
			{
				std::cout << "a_processResponse() called\n";
				//evt.ping->processResponse();
			}

			typedef ST_Pinging p; // Makes transition table cleaner
			struct transition_table : mpl::vector<
			//      Start               Event           Target              Action                      Guard
			//     +-------------------+---------------+-------------------+---------------------------+----------------------------+
			a_irow<ST_PingActive,       e_sendPing,                         &p::a_sendPing                                          >,
			a_row< ST_WaitForResponse,  e_respRcvd,     ST_WaitForResponse, &p::a_processResponse                                   >
			> {};

			PingImplementation* pingImpl;
			PingParameters*     pingParams;

		}; // ST_Pinging

		// Define the back-end of our ST_Pinging sub state machine
		typedef msm::back::state_machine<ST_Pinging> t_pingingStm;

		// The initial state of STM_Initialized
		typedef ST_Idle initial_state;



		typedef STM_Initialized i; // Makes transition table cleaner

		struct transition_table : mpl::vector<
		//      Start       Event        Target        Action                      Guard
		//     +-----------+------------+-------------+---------------------------+----------------------------+
		_row < ST_Idle,     e_startPing, t_pingingStm                                                          >,
		_row < ST_Pinging,  e_stopPing,  ST_Idle                                                               >
		> {};

		PingImplementation* pingImpl;
	}; // struct STM_Initialized

	// The initial state of the Ping SM
	typedef ST_Uninit initial_state;

	// Transition actions
	void a_initialize(e_init const& evt)
	{
		std::cout << "a_initialize() called\n";
	}
	void a_uninitialize(e_uninit const& evt)
	{
		std::cout << "a_uninitialize() called\n";
	}
	void a_setTarget(e_setTarget const& evt)
	{
		std::cout << "a_setTarget() called\n";
		//evt.ping->setTarget(evt.target);
	}
	void a_setTTL(e_setTTL const& evt)
	{
		std::cout << "a_setTTL() called\n";
		//evt.ping->setTTL(evt.TTL);
	}
	void a_setTimeout(e_setTimeout const& evt)
	{
		std::cout << "a_setTimeout() called\n";
		//evt.ping->setRecvTimeout(evt.timeout);
	}
	void a_setCount(e_setCount const& evt)
	{
		std::cout << "a_setCount() called\n";
		//evt.ping->setCount(evt.count);
	}
	void a_setInterval(e_setInterval const& evt)
	{
		std::cout << "a_setInterval() called\n";
		//evt.ping->setInterval(evt.interval);
	}
	void a_setSize(e_setSize const& evt)
	{
		std::cout << "a_setSize() called\n";
		//evt.ping->setSize(evt.size);
	}

	struct transition_table : mpl::vector<
	//      Start             Event          Target            Action                      Guard
	//     +-----------------+--------------+-----------------+---------------------------+----------------------------+
	a_row< ST_Uninit,         e_init,        ST_Initializing,   a_initialize                                      >,
	_row< ST_Initializing,    e_initSuccess, STM_Initialized                                                      >,
	a_row< STM_Initialized,   e_uninit,      ST_Uninit,         a_uninitialize                                    >,
	a_irow<STM_Initialized,   e_setTarget,                      a_setTarget                                       >,
	a_irow<STM_Initialized,   e_setTTL,                         a_setTTL                                          >,
	a_irow<STM_Initialized,   e_setTimeout,                     a_setTimeout                                      >,
	a_irow<STM_Initialized,   e_setCount,                       a_setCount                                        >,
	a_irow<STM_Initialized,   e_setInterval,                    a_setInterval                                     >,
	a_irow<STM_Initialized,   e_setSize,                        a_setSize                                         >
	> {};

	PingImplementation* pingImpl;
	PingParameters*     pingParams;
}; // STM_Ping


// Define the back-end, it's all the referring elements need
typedef msm::back::state_machine<STM_Ping> t_pingStm;

#endif /* PINGSTATEMACHINE_HPP_ */
