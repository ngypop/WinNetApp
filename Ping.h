/*
 * Ping.h
 *
 *  Created on: 01.09.2018
 *      Author: Ingo
 */

#ifndef PING_H_
#define PING_H_

#include <boost/msm/front/state_machine_def.hpp>
#include <boost/msm/back/state_machine.hpp>
#include <boost/mpl/vector.hpp>

#include <winsock2.h>
#include "Service.h"

namespace msm = boost::msm; // Meta state machine
namespace mpl = boost::mpl; // Meta programming library




class Ping : public Service
{
public:
	Ping();
	void executeCommand(string _command);

private:
	void printHelpText();
	void initialize();
	void uninitialize();
	void setTarget(string _target);
	void setRecvTimeout(unsigned int _recvTimeout);
	void setTTL(unsigned int _ttl);

	SOCKET pingSocket;


	struct sockaddr_in source, dest;
	unsigned int ttl;
	unsigned int recvTimeout;
	string targetName;

	///////////////////////////////
	// State chart definitions   //
	///////////////////////////////

	// Events
	struct e_init
	{
		e_init(Ping *_ping)
		: ping(_ping)
		{}
		Ping* ping;
	};
	struct e_setTarget
	{
		e_setTarget(Ping *_ping, string _target)
		: ping(_ping), target(_target)
		{}
		Ping* ping;
		string target;
	};
	struct e_setTTL
	{
		e_setTTL(Ping *_ping, unsigned char _TTL)
		: ping(_ping), TTL(_TTL)
		{}
		Ping* ping;
		unsigned char TTL;
	};
	struct e_setTimeout
	{
		e_setTimeout(Ping *_ping, unsigned int _timeout)
		: ping(_ping), timeout(_timeout)
		{}
		Ping* ping;
		unsigned int timeout;
	};
	struct e_startPing {};
	struct e_stopPing {};
	struct e_uninit
	{
		e_uninit(Ping *_ping)
		: ping(_ping)
		{}

		Ping* ping;
	};

	// Define the FSM structure for our Ping state machine
	struct STM_Ping : public msm::front::state_machine_def<STM_Ping>
	{
		template <class Event,class FSM>
		void on_entry(Event const& ,FSM&)
		{
			std::cout << "entering: state machine STM_Ping" << std::endl;
		}
		template <class Event,class FSM>
		void on_exit(Event const&,FSM& )
		{
			std::cout << "state machine STM_Ping" << std::endl;
		}

		// The list of FSM states
		struct ST_Uninit : public msm::front::state<>
		{
			// every (optional) entry/exit methods get the event passed.
			template <class Event,class FSM>
			void on_entry(Event const&,FSM& ) {std::cout << "entering: ST_Uninit" << std::endl;}
			template <class Event,class FSM>
			void on_exit(Event const&,FSM& ) {std::cout << "leaving: ST_Uninit" << std::endl;}
		};

		// Sub state machine STM_Initialised
		struct STM_Initialised : public msm::front::state_machine_def<STM_Initialised>
		{
			struct ST_Idle : public msm::front::state<>
			{
				template <class Event,class FSM>
				void on_entry(Event const& ,FSM&) {std::cout << "entering: ST_Idle" << std::endl;}
				template <class Event,class FSM>
				void on_exit(Event const&,FSM& ) {std::cout << "leaving: ST_Idle" << std::endl;}
			};

			struct ST_Pinging : public msm::front::state<>
			{
				template <class Event,class FSM>
				void on_entry(Event const& ,FSM&) {std::cout << "entering: ST_Pinging" << std::endl;}
				template <class Event,class FSM>
				void on_exit(Event const&,FSM& ) {std::cout << "leaving: ST_Pinging" << std::endl;}
			};

			// The initial state of the Initialised SM
			typedef ST_Idle initial_state;



			typedef STM_Initialised i; // Makes transition table cleaner

			struct transition_table : mpl::vector<
			//      Start       Event        Target      Action                      Guard
			//     +-----------+------------+-----------+---------------------------+----------------------------+
			_row < ST_Idle,     e_startPing, ST_Pinging                                                          >,
			_row < ST_Pinging,  e_stopPing,  ST_Idle                                                             >
			> {};
	}; // struct STM_Initialised

		// Define the back-end of our sub state machine
		typedef msm::back::state_machine<STM_Initialised> t_initialisedStm;

		// The initial state of the Ping SM
		typedef ST_Uninit initial_state;

		// Transition actions
		void a_initialize(e_init const& evt)
		{
			std::cout << "a_initialize() called\n";
			evt.ping->initialize();
		}
		void a_uninitialize(e_uninit const& evt)
		{
			std::cout << "a_uninitialize() called\n";
			evt.ping->uninitialize();
		}
		void a_setTarget(e_setTarget const& evt)
		{
			std::cout << "a_setTarget() called\n";
			evt.ping->setTarget(evt.target);
		}
		void a_setTTL(e_setTTL const& evt)
		{
			std::cout << "a_setTTL() called\n";
			evt.ping->setTTL(evt.TTL);
		}
		void a_setTimeout(e_setTimeout const& evt)
		{
			std::cout << "a_setTimeout() called\n";
			evt.ping->setRecvTimeout(evt.timeout);
		}


		// Guard conditions

		// Transition table

		typedef STM_Ping p; // Makes transition table cleaner

		struct transition_table : mpl::vector<
		//      Start             Event          Target            Action                      Guard
		//     +-----------------+--------------+-----------------+---------------------------+----------------------------+
		a_row< ST_Uninit,         e_init,        t_initialisedStm,  &p::a_initialize                                      >,
		a_row< t_initialisedStm,  e_uninit,      ST_Uninit,         &p::a_uninitialize                                    >,
		a_irow<t_initialisedStm,  e_setTarget,                      &p::a_setTarget                                       >,
		a_irow<t_initialisedStm,  e_setTTL,                         &p::a_setTTL                                          >,
		a_irow<t_initialisedStm,  e_setTimeout,                     &p::a_setTimeout                                      >

		> {};

		// Replaces the default no-transition response.
		template <class FSM,class Event>
		void no_transition(Event const& e, FSM&,int state)
		{
			std::cout << "no transition from state " << state
					<< " on event " << typeid(e).name() << std::endl;
		}
	};

    // Define the back-end
    typedef msm::back::state_machine<STM_Ping> t_pingStm;
    t_pingStm pingStm;
};

#endif /* PING_H_ */
