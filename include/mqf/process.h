#ifndef INCLUDED_MQF_PROCESS
#define INCLUDED_MQF_PROCESS

namespace mqf {

	template<typename Value_ = double,typename Time_ = double>
	struct Process {
		using Time = Time_;
		using Value = Value_;
	};

	namespace Processes {

		template<typename Time = double>
		struct StoppingTime : Process<Time,Time> {};

	}

}

#endif
