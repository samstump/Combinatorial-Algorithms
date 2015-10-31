// backtrack.h

#ifndef _BACKTRACK_H_INCLUDED
#define _BACKTRACK_H_INCLUDED

#include <vector>
#include <stack>

template <typename ElemType>
class Strategy {
	public:
		virtual int get_candidates(std::vector<std::stack<ElemType>>& stacks, const std::vector<ElemType>& partial_soln) = 0;
		// computes a list of candidates for the next element of partial_soln (partial_soln.size()), and
		// pushes them into a stack at stacks[partial_soln.size()].  Returns the number of candidates computed.

		virtual bool is_solution(const std::vector<ElemType>& soln) const = 0;
		// returns true of soln is a solution to the problem, false otherwise

		virtual ~Strategy() {}
};

// TODO: Evaluate the function call operator (operator()).  Is this better than a named function?

template <typename ElemType>
class Accumulator {
	public:
		virtual void operator()(const std::vector<ElemType>& soln) = 0;
		// this is the 'accumulate' function

		virtual ~Accumulator() {}
};

// TODO: The 'verbose' flag in backtrack class needs some thought.  Don't depend on std::cout for example.
// TODO: Unify the logging (verbose) for all classes concerned (backtrack, Strategy and Accumulator).
// TODO: Evaluate the function call operator (operator()).  Is this better than a named function?
template <typename ElemType>
class backtrack {
	public:
		backtrack(Strategy<ElemType>& strat, Accumulator<ElemType>& accum, int solution_length);
		
		bool set_verbose(bool newValue) {
			bool oldValue = verbose;
			verbose = newValue;
			return oldValue;
		}
		
		void operator()();
		
	private:
		int									desired_solution_length;
		std::vector<ElemType>				solution;
		std::vector<std::stack<ElemType>>	candidates;
		Strategy<ElemType>&					strategy;		// to get candidates
		Accumulator<ElemType>&				accumulator;	// to recieve solutions
		bool								verbose;		// enables logging via std::cout, could be better

};

// TODO: solution_length parameter is unnecessary, desired_solution_length can be obtained from the Strategy
template <typename ElemType>
backtrack<ElemType>::backtrack(Strategy<ElemType>& strat, Accumulator<ElemType>& accum, int solution_length) : 
	desired_solution_length(solution_length), 
	solution(),
	candidates(solution_length, std::stack<ElemType>()),
	strategy(strat),
	accumulator(accum),
	verbose(false)
{
}

template <typename ElemType>
void backtrack<ElemType>::operator()()
{
	// either need this, or initial candidates initialized before the loop
	bool initial_state = true;
	
	int pos = 0;
		
	while (pos >= 0) {
		
		// get intial candidates, could be done outside loop
		if (initial_state) {
			initial_state = false;
			strategy.get_candidates(candidates, solution);
		}

		// INVARIANT: pos >= 0, solution.size() == pos	
		// while we have no candidates for a given position, backtrack...
		while (pos >= 0 && candidates[pos].size() == 0) {
			if (verbose) std::cout << "no candidates for position: " << pos << ", backtracking to " << pos - 1 << std::endl;
			if (pos > 0) {
				solution.pop_back();
			}
			pos -= 1;
		}
		// backtrack went too far, we're done.
		if (pos < 0) {
			continue;
		}
		if (verbose) std::cout << "we have candidates for position: " << pos << std::endl;

		// INVARIANT: pos >= 0, solution.size() == pos, candidates[pos].size() > 0
		// grab the top candidate for the current position, and advance ...
		ElemType c = candidates[pos].top();
		if (verbose) std::cout << "selecting candidate: " << c << " for position: " << pos << std::endl;
		candidates[pos].pop();
		
		solution.push_back(c);
		pos += 1;

		// have we built a solution?
		if (strategy.is_solution(solution)) {
			// we have a solution, accumulate it and get ready to find the next.
			accumulator(solution);
			pos -= 1;
			solution.pop_back();
		} else {
			// not a solution yet, get candidates for the next position.
			strategy.get_candidates(candidates, solution);
		}
	}
}

#endif //_BACKTRACK_H_INCLUDED