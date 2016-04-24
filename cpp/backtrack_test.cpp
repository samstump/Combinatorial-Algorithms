// backtrack_test.cpp

#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
//#include <chrono>

#include "timer.h"
#include "backtrack.h"

std::ostream& operator<<(std::ostream& o, const std::vector<int>& a)
{
	o << "[";
	for (std::vector<int>::const_iterator it = a.begin(); it != a.end(); ++it) {
		o << *it;
		if (it + 1 != a.end()) o << ", ";
	}
	return o << "]";
}

// vectors with sum of components even and greater than 'vectorSum'
class VectorOfGivenSizeAndSumStrategy : public Strategy<int> {
	public:
		VectorOfGivenSizeAndSumStrategy(int n, int sum) : 
			vectorSize(n), 
			vectorSum(sum),
			verbose(false)
		{
		}
		
		bool set_verbose(bool newValue)
		{
			bool oldValue = verbose;
			verbose = newValue;
			return oldValue;
		}
		
		int get_candidates(std::vector<std::stack<int>>& stacks, const std::vector<int>& partial_soln)
		{
			if (verbose) std::cout << "partial vector: " << partial_soln << std::endl;
			const std::vector<int> all_candidates = {1,2,3,4,5,6,7,8,9};
			int partial_sum = 0;
			for (std::vector<int>::const_iterator it = partial_soln.begin(); it != partial_soln.end(); ++it) {
				partial_sum += *it;
			}
			int pos = partial_soln.size();
			if (verbose) std::cout << "candidates for position: " << pos << std::endl << "[";
			int num_left = stacks.size() - partial_soln.size();
			if (num_left == 1) {				// last position, be careful ...
				for (std::vector<int>::const_iterator it = all_candidates.begin(); it != all_candidates.end(); ++it) {
					int candidate = *it;
					int temp = partial_sum + candidate;
					if (temp > vectorSum && temp % 2 == 0) {
						stacks[pos].push(candidate);
						if (verbose) std::cout << candidate << ",";
					}
				}
			} else {
				for (std::vector<int>::const_iterator it = all_candidates.begin(); it != all_candidates.end(); ++it) {
					int candidate = *it;
					stacks[pos].push(candidate);
					if (verbose) std::cout << candidate << ",";
				}
			}
			if (verbose) std::cout << "]" << std::endl;
			return stacks[pos].size();
		}

		bool is_solution(const std::vector<int>& solution) const
		{
			return solution.size() == vectorSize;
		}
	
	private:
		int vectorSize;
		int vectorSum;
		bool verbose;
};


// vectors with sum of components a multiple of N
class VectorSumIsMultN : public Strategy<int> {
	public:
		VectorSumIsMultN(int l, int n) : 
			vectorSize(l), 
			vectorMult(n),
			verbose(false)
		{
		}
		
		bool set_verbose(bool newValue)
		{
			bool oldValue = verbose;
			verbose = newValue;
			return oldValue;
		}
		
		int get_candidates(std::vector<std::stack<int>>& stacks, const std::vector<int>& partial_soln)
		{
			if (verbose) std::cout << "partial vector: " << partial_soln << std::endl;
			const std::vector<int> all_candidates = {1,2,3,4,5,6,7,8,9};
			//const std::vector<int> all_candidates = {9,8,7,6,5,4,3,2,1};
			int partial_sum = 0;
			for (std::vector<int>::const_iterator it = partial_soln.begin(); it != partial_soln.end(); ++it) {
				partial_sum += *it;
				partial_sum = partial_sum % vectorMult;
			}
			int pos = partial_soln.size();
			if (verbose) std::cout << "candidates for position: " << pos << std::endl << "[";
			int num_left = stacks.size() - partial_soln.size();
			if (num_left == 1) {				// last position, be careful ...
				for (std::vector<int>::const_iterator it = all_candidates.begin(); it != all_candidates.end(); ++it) {
					int candidate = *it;
					int temp = partial_sum + candidate;
					if (temp % vectorMult == 0) {
						stacks[pos].push(candidate);
						if (verbose) std::cout << candidate << ",";
					}
				}
			} else {
				for (std::vector<int>::const_iterator it = all_candidates.begin(); it != all_candidates.end(); ++it) {
					int candidate = *it;
					stacks[pos].push(candidate);
					if (verbose) std::cout << candidate << ",";
				}
			}
			if (verbose) std::cout << "]" << std::endl;
			return stacks[pos].size();
		}

		bool is_solution(const std::vector<int>& solution) const
		{
			return solution.size() == vectorSize;
		}
	
	private:
		int vectorSize;
		int	vectorMult;
		bool verbose;
};


class OutputAccumulator : public Accumulator<int> {
	public:
		OutputAccumulator(std::ostream& o) : 
			ostr(o),
			num_solutions(0)
		{
		}

		void operator()(const std::vector<int>& soln)
		{
			std::cout << "solution " << ++num_solutions << ": " << soln << std::endl;
		}

		int solution_count() const { return num_solutions; }

	private:
		int num_solutions;
		std::ostream& ostr;
	
};

int main()
{
	timer t;
	
	t.start();
	
	bool verbose = false;
	int desired_solution_len = 4;
	
	OutputAccumulator output(std::cout);
	
//	VectorOfGivenSizeAndSumStrategy strat(3,10);
	VectorSumIsMultN strat(desired_solution_len, 5);
	strat.set_verbose(verbose);
	
	backtrack<int> back(strat, output, desired_solution_len);
	back.set_verbose(verbose);
	
	back();

	timer::duration d = t.stop();
	std::cout << "elapsed time = " << d.count() << " ns" << std::endl;
	std::cout << "elapsed time = " << timer::to_microseconds(d) << " us" << std::endl;
	std::cout << "elapsed time = " << timer::to_milliseconds(d) << " ms" << std::endl;
	std::cout << "elapsed time = " << timer::to_microseconds(d)/output.solution_count() << " us/solution" << std::endl;
}
