// graph.cpp

#include <iostream>
#include <algorithm>
#include "graph.h"
#include "fast_set.h"

std::ostream& operator<<(std::ostream& o, const edge_type& e)
{
	return o << "(" << e.first << "," << e.second << ")";
}

void graph::insert_into_deg_vertex_set(int deg, int v)
{
//	std::cout << "inserting vertex: " << v << " of degree " << deg << std::endl;
	deg_vertex_set[deg].insert(v);
}

void graph::remove_from_deg_vertex_set(int deg, int v)
{
//	std::cout << "removing vertex: " << v << " of degree " << deg << std::endl;
	deg_vertex_set[deg].remove(v);
}

void graph::insert_into_adj_matrix(int a, int b, int edge_index)
{
	int v0 = std::min(a,b);
	int v1 = std::max(a,b);
	adj_matrix[v0 + v1*(v1-1)/2] = edge_index;
}

int graph::get_from_adj_matrix(int a, int b)
{
	int v0 = std::min(a,b);
	int v1 = std::max(a,b);
	return adj_matrix[v0 + v1*(v1-1)/2];
}

graph::graph(int n, const edge_list& E) : 
	num_vertices(n), num_edges(E.size()), edges(E), weights(E.size(), 1), degrees(n, 0), 
	adj_matrix(n*(n-1)/2, -1), inc_list(n, std::vector<int>()), deg_vertex_set(n*(n-1) + 1, fast_set<uint16_t>(num_vertices)),
	vertex_priority(num_vertices), edge_priority(num_edges)
{
	for (int edge_index = 0; edge_index < num_edges; ++edge_index) {
		int v0 = edges[edge_index].first;
		int v1 = edges[edge_index].second;
		degrees[v0] += weights[edge_index];
		degrees[v1] += weights[edge_index];
		inc_list[v0].push_back(edge_index);
		inc_list[v1].push_back(edge_index);
		insert_into_adj_matrix(v0,v1,edge_index);
	}
	for (int vertex_index = 0; vertex_index < num_vertices; ++vertex_index) {
		insert_into_deg_vertex_set(degrees[vertex_index], vertex_index);
	}
}

int graph::adjust_weight(int edge_index, int new_weight)
{
	int old_weight = weights[edge_index];
	
	if (old_weight == new_weight) return old_weight;
	
	int v0 = edges[edge_index].first;
	int v1 = edges[edge_index].second;
	
	{
		int dv0 = degrees[v0];
		int dv1 = degrees[v1];
		
		remove_from_deg_vertex_set(dv0, v0);
		remove_from_deg_vertex_set(dv1, v1);
	}
	degrees[v0] += (new_weight - old_weight);
	degrees[v1] += (new_weight - old_weight);
	
	weights[edge_index] = new_weight;
	
	{
		int dv0 = degrees[v0];
		int dv1 = degrees[v1];

		insert_into_deg_vertex_set(dv0, v0);
		insert_into_deg_vertex_set(dv1, v1);
	}
	
	return old_weight;
}

int graph::compute_edge_priorities()
{
	for (int i = 0; i < deg_vertex_set.size(); ++i) {
		for (int j = 0; j < deg_vertex_set[i].size(); ++j) {
			vertex_priority[deg_vertex_set[i][j]] = std::pair<int,int>(deg_vertex_set[i].size(), i);
		}
	}
	
	for (int i = 0; i < num_edges; ++i) {
		std::pair<int,int> p0 = vertex_priority[edges[i].first];
		std::pair<int,int> p1 = vertex_priority[edges[i].second];
		edge_priority[i].first = (p0.second != p1.second ? p1.first * p0.first : 0);
		edge_priority[i].second = i;
	}
	std::sort(edge_priority.begin(), edge_priority.end(), std::greater<std::pair<int,int>>());
	return edge_priority[0].second;
}

std::ostream& graph::display(std::ostream& o)
{
	o << num_vertices << " vertices, " << num_edges << " edges: E=" << edges << std::endl;
	o << "weights: " << weights << std::endl;
	o << "degrees: " << degrees << std::endl;
	if (is_irregular()) {
		o << "irregular, s(G)=" << s() << std::endl;
	} else {
		o << "not irregular" << std::endl;
	}
	o << "inc list:\n";
	for (int v = 0; v < inc_list.size(); ++v) {
		o << "  v=" << v << ": ";
		for (int ei = 0; ei < inc_list[v].size(); ++ei) {
			o << "e=" << inc_list[v][ei] << " " << edges[inc_list[v][ei]];
			if (ei < inc_list[v].size() - 1) o << ", ";
		}
		o << std::endl;
	}
	o << "degree map:\n";
	for (int d = 0; d < deg_vertex_set.size(); ++d) {
		if (deg_vertex_set[d].size() > 0) {
			o << "  degree: " << d << ": " << deg_vertex_set[d] << std::endl;
		}
	}
	
	std::vector<std::pair<int,int>> sorted_deg_vertex_set;
	
	for (int d = 0; d < deg_vertex_set.size(); ++d) {
		sorted_deg_vertex_set.push_back(std::pair<int,int>(deg_vertex_set[d].size(), d));
	}
	std::sort(sorted_deg_vertex_set.begin(), sorted_deg_vertex_set.end(), std::greater<std::pair<int,int>>());
	o << "sorted degree map:\n";
	for (int d = 0; d < sorted_deg_vertex_set.size(); ++d) {
		if (sorted_deg_vertex_set[d].first > 0) {
			o << "  degree: " << sorted_deg_vertex_set[d].second << ": " << deg_vertex_set[sorted_deg_vertex_set[d].second] << std::endl;
		}
	}

	o << "Vertex Priority: (set size, degree)" << std::endl;
	for (int i = 0; i < num_vertices; ++i) {
		o << i << " : " << vertex_priority[i] << std::endl;
	}
	o << "Edge Priority:" << std::endl;
	for (int i = 0; i < num_edges; ++i) {
		o << edge_priority[i].second << " : " << edges[edge_priority[i].second] << " : " << edge_priority[i].first << std::endl;
	}
	return o;
}
