#include "doctest.h"

#include <iostream>
#include <sstream>

#include "io.hpp"
#include "uni/space.hpp"

using namespace NP;

static const auto inf = Time_model::constants<dtime_t>::infinity();


// The example in Fig 1 of Nasri & Fohler (ECRTS 2016):
//    "Non-Work-Conserving Non-Preemptive Scheduling:
//     Motivations, Challenges, and Potential Solutions"
TEST_CASE("[IIP] P-RM example (Figure 1)")
{
	Uniproc::State_space<dtime_t>::Workload jobs{
		// high-frequency task tau_1
		Job<dtime_t>{1, I( 0,  0), I(1, 1), 10, 1, 1},
		Job<dtime_t>{2, I(10, 10), I(1, 1), 20, 1, 1},
		Job<dtime_t>{3, I(20, 20), I(1, 1), 30, 1, 1},
		Job<dtime_t>{4, I(30, 30), I(1, 1), 40, 1, 1},
		Job<dtime_t>{5, I(40, 40), I(1, 1), 50, 1, 1},
		Job<dtime_t>{6, I(50, 50), I(1, 1), 60, 1, 1},

		// middle task
		Job<dtime_t>{1, I( 0,  0), I(8, 8), 30, 2, 2},
		Job<dtime_t>{2, I(30, 30), I(8, 8), 60, 2, 2},

		// the long task
		Job<dtime_t>{1, I( 0,  0), I(17, 17), 60, 3, 3}
	};

	SUBCASE("RM, naive exploration") {
		auto space = Uniproc::State_space<dtime_t>::explore_naively(jobs);
		CHECK(!space.is_schedulable());
	}

	SUBCASE("RM, exploration with state-merging") {
		auto space = Uniproc::State_space<dtime_t>::explore(jobs);
		CHECK(!space.is_schedulable());
	}

	SUBCASE("P-RM, naive exploration") {
		auto space = Uniproc::State_space<dtime_t, Uniproc::Precatious_RM_IIP<dtime_t>>::explore_naively(jobs);
		CHECK(space.is_schedulable());
	}

	SUBCASE("P-RM, exploration with state-merging") {
		auto space = Uniproc::State_space<dtime_t, Uniproc::Precatious_RM_IIP<dtime_t>>::explore(jobs);
		CHECK(space.is_schedulable());
	}

}

// The example in Fig 2a of Nasri & Fohler (ECRTS 2016):
//    "Non-Work-Conserving Non-Preemptive Scheduling:
//     Motivations, Challenges, and Potential Solutions"
TEST_CASE("[IIP] P-RM negative example (Figure 2)")
{
	Uniproc::State_space<dtime_t>::Workload jobs{
		// high-frequency task tau_1
		Job<dtime_t>{1, I( 0,  0), I(3, 3), 10, 1, 1},
		Job<dtime_t>{2, I(10, 10), I(3, 3), 20, 1, 1},
		Job<dtime_t>{3, I(20, 20), I(3, 3), 30, 1, 1},
		Job<dtime_t>{4, I(30, 30), I(3, 3), 40, 1, 1},
		Job<dtime_t>{5, I(40, 40), I(3, 3), 50, 1, 1},
		Job<dtime_t>{6, I(50, 50), I(3, 3), 60, 1, 1},

		// middle task
		Job<dtime_t>{1, I( 0,  0), I(6, 6), 12, 2, 2},
		Job<dtime_t>{2, I(12, 12), I(6, 6), 24, 2, 2},
		Job<dtime_t>{3, I(24, 24), I(6, 6), 36, 2, 2},
		Job<dtime_t>{4, I(36, 36), I(6, 6), 48, 2, 2},
		Job<dtime_t>{5, I(48, 48), I(6, 6), 60, 2, 2},

		// the long task
		Job<dtime_t>{1, I( 0,  0), I(8, 8), 60, 3, 3}
	};

	SUBCASE("RM, naive exploration") {
		auto space = Uniproc::State_space<dtime_t>::explore_naively(jobs);
		CHECK(!space.is_schedulable());
	}

	SUBCASE("RM, exploration with state-merging") {
		auto space = Uniproc::State_space<dtime_t>::explore(jobs);
		CHECK(!space.is_schedulable());
	}

	SUBCASE("P-RM, naive exploration") {
		auto space = Uniproc::State_space<dtime_t, Uniproc::Precatious_RM_IIP<dtime_t>>::explore_naively(jobs);
		CHECK(!space.is_schedulable());
	}

	SUBCASE("P-RM, exploration with state-merging") {
		auto space = Uniproc::State_space<dtime_t, Uniproc::Precatious_RM_IIP<dtime_t>>::explore(jobs);
		CHECK(!space.is_schedulable());
	}

}


// The example in Fig 2b of Nasri & Fohler (ECRTS 2016):
//    "Non-Work-Conserving Non-Preemptive Scheduling:
//     Motivations, Challenges, and Potential Solutions"
TEST_CASE("[IIP] CW-EDF example (Figure 2)")
{
	Uniproc::State_space<dtime_t>::Workload jobs{
		// high-frequency task tau_1
		Job<dtime_t>{1, I( 0,  0), I(3, 3), 10, 10, 1},
		Job<dtime_t>{2, I(10, 10), I(3, 3), 20, 20, 1},
		Job<dtime_t>{3, I(20, 20), I(3, 3), 30, 30, 1},
		Job<dtime_t>{4, I(30, 30), I(3, 3), 40, 40, 1},
		Job<dtime_t>{5, I(40, 40), I(3, 3), 50, 50, 1},
		Job<dtime_t>{6, I(50, 50), I(3, 3), 60, 60, 1},

		// middle task
		Job<dtime_t>{1, I( 0,  0), I(6, 6), 12, 12, 2},
		Job<dtime_t>{2, I(12, 12), I(6, 6), 24, 24, 2},
		Job<dtime_t>{3, I(24, 24), I(6, 6), 36, 36, 2},
		Job<dtime_t>{4, I(36, 36), I(6, 6), 48, 48, 2},
		Job<dtime_t>{5, I(48, 48), I(6, 6), 60, 60, 2},

		// the long task
		Job<dtime_t>{1, I( 0,  0), I(8, 8), 60, 60, 3}
	};

	SUBCASE("EDF, naive exploration") {
		auto space = Uniproc::State_space<dtime_t>::explore_naively(jobs);
		CHECK(!space.is_schedulable());
	}

	SUBCASE("EDF, exploration with state-merging") {
		auto space = Uniproc::State_space<dtime_t>::explore(jobs);
		CHECK(!space.is_schedulable());
	}

	SUBCASE("CW-EDF, naive exploration") {
		auto space = Uniproc::State_space<dtime_t, Uniproc::Critical_window_IIP<dtime_t>>::explore_naively(jobs);
		CHECK(space.is_schedulable());
	}

	SUBCASE("CW-EDF, exploration with state-merging") {
		auto space = Uniproc::State_space<dtime_t, Uniproc::Critical_window_IIP<dtime_t>>::explore(jobs);
		CHECK(space.is_schedulable());
	}

}


TEST_CASE("[IIP] CW-EDF extra example")
{
	Uniproc::State_space<dtime_t>::Workload jobs{
		// high-frequency task tau_1
		Job<dtime_t>{1, I( 0,  0), I(3, 3), 10, 10, 1},
		Job<dtime_t>{2, I(10, 10), I(3, 3), 20, 20, 1},
		Job<dtime_t>{3, I(20, 20), I(3, 3), 30, 30, 1},
		Job<dtime_t>{4, I(30, 30), I(3, 3), 40, 40, 1},
		Job<dtime_t>{5, I(40, 40), I(3, 3), 50, 50, 1},
		Job<dtime_t>{6, I(50, 50), I(3, 3), 60, 60, 1},

		// middle task
		Job<dtime_t>{1, I( 0,  0), I(8, 8), 15, 15, 2},
		Job<dtime_t>{2, I(15, 15), I(8, 8), 30, 30, 2},
		Job<dtime_t>{3, I(30, 30), I(8, 8), 45, 45, 2},
		Job<dtime_t>{4, I(45, 45), I(8, 8), 60, 60, 2},

		// the long task
		Job<dtime_t>{1, I( 0,  0), I(8, 8), 60, 60, 3}
	};

	SUBCASE("EDF, naive exploration") {
		auto space = Uniproc::State_space<dtime_t>::explore_naively(jobs);
		CHECK(!space.is_schedulable());
	}

	SUBCASE("EDF, exploration with state-merging") {
		auto space = Uniproc::State_space<dtime_t>::explore(jobs);
		CHECK(!space.is_schedulable());
	}

	SUBCASE("CW-EDF, naive exploration") {
		auto space = Uniproc::State_space<dtime_t, Uniproc::Critical_window_IIP<dtime_t>>::explore_naively(jobs);
		CHECK(space.is_schedulable());
	}

	SUBCASE("CW-EDF, exploration with state-merging") {
		auto space = Uniproc::State_space<dtime_t, Uniproc::Critical_window_IIP<dtime_t>>::explore(jobs);
		CHECK(space.is_schedulable());
	}

}


TEST_CASE("[IIP] P-RM idle time")
{
	Uniproc::State_space<dtime_t>::Workload jobs{
		// high-frequency task
		Job<dtime_t>{1, I( 0,  0), I(3, 3), 10, 1, 1},
		Job<dtime_t>{2, I(10, 10), I(3, 3), 20, 1, 1},
		Job<dtime_t>{3, I(20, 20), I(3, 3), 30, 1, 1},

		// blocking job
		Job<dtime_t>{1, I( 8,  8), I(10, 10), 30, 2, 2},

		// low-priority job
		Job<dtime_t>{1, I( 9,  9), I(1, 1), 30, 3, 3},
	};

	SUBCASE("naive exploration") {
		auto space = Uniproc::State_space<dtime_t, Uniproc::Precatious_RM_IIP<dtime_t>>::explore_naively(jobs);
		CHECK(space.is_schedulable());
	}

	SUBCASE("exploration with state-merging") {
		auto space = Uniproc::State_space<dtime_t, Uniproc::Precatious_RM_IIP<dtime_t>>::explore(jobs);
		CHECK(space.is_schedulable());
	}

}

const std::string cw_edf_unschedulable_file =
	"Task ID,Job ID,Arrival min,Arrival max,Cost min,Cost max,Deadline,Priority\n"
	"1,1,0,0,0,60,200,200\n"
	"1,2,200,200,0,60,400,400\n"
	"1,3,400,400,0,60,600,600\n"
	"1,4,600,600,0,60,800,800\n"
	"1,5,800,800,0,60,1000,1000\n"
	"1,6,1000,1000,0,60,1200,1200\n"
	"1,7,1200,1200,0,60,1400,1400\n"
	"1,8,1400,1400,0,60,1600,1600\n"
	"1,9,1600,1600,0,60,1800,1800\n"
	"1,10,1800,1800,0,60,2000,2000\n"
	"2,1,0,0,0,171,2000,2000\n"
	"3,1,0,0,0,6,200,200\n"
	"3,2,200,200,0,6,400,400\n"
	"3,3,400,400,0,6,600,600\n"
	"3,4,600,600,0,6,800,800\n"
	"3,5,800,800,0,6,1000,1000\n"
	"3,6,1000,1000,0,6,1200,1200\n"
	"3,7,1200,1200,0,6,1400,1400\n"
	"3,8,1400,1400,0,6,1600,1600\n"
	"3,9,1600,1600,0,6,1800,1800\n"
	"3,10,1800,1800,0,6,2000,2000\n"
	"4,1,0,0,0,96,200,200\n"
	"4,2,200,200,0,96,400,400\n"
	"4,3,400,400,0,96,600,600\n"
	"4,4,600,600,0,96,800,800\n"
	"4,5,800,800,0,96,1000,1000\n"
	"4,6,1000,1000,0,96,1200,1200\n"
	"4,7,1200,1200,0,96,1400,1400\n"
	"4,8,1400,1400,0,96,1600,1600\n"
	"4,9,1600,1600,0,96,1800,1800\n"
	"4,10,1800,1800,0,96,2000,2000\n"
	"5,1,0,0,0,1,1000,1000\n"
	"5,2,1000,1000,0,1,2000,2000\n";

TEST_CASE("[IIP] CW-EDF infeasible example") {
	auto in = std::istringstream(cw_edf_unschedulable_file);

	Scheduling_problem<dtime_t> prob{parse_file<dtime_t>(in)};

	Analysis_options opts;

	opts.be_naive = true;
	auto nspace = Uniproc::State_space<dtime_t, Uniproc::Critical_window_IIP<dtime_t>>::explore(prob, opts);
	CHECK(!nspace.is_schedulable());

	opts.be_naive = false;
	auto space = Uniproc::State_space<dtime_t, Uniproc::Critical_window_IIP<dtime_t>>::explore(prob, opts);
	CHECK(!space.is_schedulable());
}
