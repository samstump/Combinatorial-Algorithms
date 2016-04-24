// graph.h

#include <vector>
#include <algorithm>

#include "fast_set.h"

typedef std::pair<int,int>					edge_type;
typedef std::vector<edge_type>				edge_list;
typedef std::vector<int>					weights_list;				// for each edge, weight of the edge
typedef std::vector<int>					degree_list;				// for each vertex, deg(v)
typedef std::vector<std::vector<int>>		incidence_list;				// for each vertex, list of incident edge_indices
//typedef std::vector<fast_integer_set>		degree_vertex_set;			// for each degree, d, the {v in V(G), with deg(v) = d}
typedef std::vector<fast_set<uint16_t>>		degree_vertex_set;			// for each degree, d, the {v in V(G), with deg(v) = d}
typedef std::vector<int>					adjacency_matrix;			// adjacency matrix, compressed to lower triangular, with edge index or -1

template <typename T>
std::ostream& operator<<(std::ostream& o, const fast_set<T>& v)
{
	o << "{";
	for (int i = 0; i < v.size(); ++i) {
		o << (int)v[i];
		if (i < v.size() - 1) o << ", ";
	}
	return o << "}";
}

std::ostream& operator<<(std::ostream& o, const edge_type& e);

template <typename T>
std::ostream& operator<<(std::ostream& o, const std::vector<T>& v)
{
	o << "[";
	for (int i = 0; i < v.size(); ++i) {
		o << v[i];
		if (i < v.size() - 1) o << ", ";
	}
	return o << "]";
}

class graph {
	public:
		graph(int n, const edge_list& E);
		
		const std::pair<int,int> get_ve() const
		{
			return std::pair<int,int>(num_vertices, num_edges);
		}
		
		const int get_upper_bound_on_s_G() const
		{
			return num_vertices - 1;
		}
		
		bool is_irregular() const
		{
			// if a degree is shared by more than 1 vertex, it is NOT an irregular assignment
			auto size_gt_1 = [](auto x) { return x.size() > 1; };
			auto it = std::find_if(deg_vertex_set.begin(), deg_vertex_set.end(), size_gt_1);
			return it == deg_vertex_set.end();
		}
		
		int get_weight(int edge_index) const
		{
			return weights[edge_index];
		}
		
		int adjust_weight(int edge_index, int new_weight);
		int compute_edge_priorities();
		int s() const { return *(std::max_element(weights.begin(), weights.end())); }
		std::ostream& display(std::ostream& o);
	
	private:
		int 				num_vertices;
		int 				num_edges;
		edge_list			edges;
		weights_list		weights;
		degree_list			degrees;
		incidence_list		inc_list;
		degree_vertex_set	deg_vertex_set;
		adjacency_matrix	adj_matrix;
		std::vector<std::pair<int,int>> vertex_priority;
		std::vector<std::pair<int,int>> edge_priority;
		
		
		void insert_into_deg_vertex_set(int deg, int v);
		void remove_from_deg_vertex_set(int deg, int v);
		
		void insert_into_adj_matrix(int v0, int v1, int edge_index);
		int get_from_adj_matrix(int v0, int v1);
		
		

};

