/*
Copyright (c) 2017-2019 Origin Quantum Computing. All Right Reserved.
Licensed under the Apache License 2.0

QProgDAG.h
Author: doumenghan
Updated in 2019/08/06 

Classes for QProgDAG.

*/
/*! \file QProgDAG.h */
#ifndef  QPROGDAG_H_
#define  QPROGDAG_H_

#include <vector>
#include <memory>
#include "Core/QuantumCircuit/QGate.h"
#include "Core/QuantumCircuit/QCircuit.h"
#include "Core/QuantumCircuit/QProgram.h"
#include "Core/QuantumCircuit/QuantumMeasure.h"
#include "Core/QuantumCircuit/QGlobalVariable.h"
#include "ThirdParty/Eigen/Sparse"
#include "ThirdParty/Eigen/Dense"

QPANDA_BEGIN


enum SequenceNodeType
{
	MEASURE = -1,
	RESET = -2
};

struct SequenceNode
{
    int m_node_type; // SequenceNodeType(on case of m_node_type < 0) and GateType
    size_t m_vertex_num;

    bool operator == (const SequenceNode &node) const { return (this->m_vertex_num == node.m_vertex_num); }
    bool operator <  (const SequenceNode &node) const { return (this->m_vertex_num  < node.m_vertex_num); }
    bool operator >  (const SequenceNode &node) const { return (this->m_vertex_num  > node.m_vertex_num); }
};

using edges_vec = std::vector<std::pair<size_t, size_t>>; 
using AdjacencyMatrix = Eigen::MatrixXi;
using LayerNode = std::pair<SequenceNode, std::vector<SequenceNode>>;
using SequenceLayer = std::vector<LayerNode>;
using TopologicalSequence = std::vector<SequenceLayer>;


/**
* @class QProgDAG
* @ingroup Utilities
* @brief transform QProg to DAG(directed acyclic graph)
* @note
*/
class QProgDAG
{
public:
	struct NodeInfo
	{
		NodeInfo(const NodeIter itr)
			:m_itr(itr), m_dagger(false)
		{}

		NodeIter m_itr;
		bool m_dagger;
		QVec m_qubits_vec;
		QVec m_control_vec;
	};
	using vertices_map = std::map<size_t, NodeInfo>;
    std::vector<size_t> m_qubit_vec;

	QProgDAG() {}

    /**
    * @brief  get TopologincalSequence
    * @param[out]  TopologicalSequence 
    * @return     void
    */
    void getTopologincalSequence(TopologicalSequence &);

	/**
	* @brief  add vertex
	* @param[in]  node  quantum node
	* @return     size_t vertex num
	*/
	size_t add_vertex(std::shared_ptr<QNode> node);

    /**
    * @brief  add vertex
    * @param[in]  node_info 
    * @return     size_t vertex num
    */
    size_t add_vertex(const NodeInfo& node_info);

    /**
    * @brief  add edge
    * @param[in]  size_t vertex num
    * @param[in]  size_t vertex num
    * @return     void
    */
    void add_edge(size_t,size_t);
    
    /**
    * @brief  get adjacency_matrix
    * @param[in]   vertices_map&
    * @param[out]  AdjacencyMatrix&
    * @return     void
    */
    void get_adjacency_matrix(const vertices_map &, AdjacencyMatrix &);

    /**
    * @brief  construct sequence node
    * @param[in]  size_t vertex num
    * @return     QPanda::SequenceNode
    */
    SequenceNode construct_sequence_node(size_t);

    /**
    * @brief  get vertex by vertex num
    * @param[in]  size_t vertex num
    * @return     std::shared_ptr<QPanda::QNode> qnode
    */
    std::shared_ptr<QNode> get_vertex(size_t) const;

	/**
	* @brief  get vertex nodeIter by vertex num
	* @param[in]  size_t vertex num
	* @return     QPanda::NodeIter
	*/
	NodeIter get_vertex_nodeIter(size_t) const;

    /**
    * @brief  add qubit map
    * @param[in]  size_t qubit
    * @param[in]  size_t vertex num
    * @return     void
    */
    void add_qubit_map(size_t, size_t);

    bool is_connected_graph();

private:
    QProg m_dag_prog;
    edges_vec m_edges_vector;
    vertices_map m_vertices_map;
    std::map<size_t, std::vector<size_t>> qubit_vertices_map;

    void _get_cur_layer_vertices(AdjacencyMatrix &,SequenceLayer &);
};

QPANDA_END
#endif
