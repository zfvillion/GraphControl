/*******************************************************************************
 * Copyright (C) 2011 HPCC Systems.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 ******************************************************************************/
#include "precompiled_headers.h"

#include "GraphCluster.h"

namespace hpcc
{
CCluster::CCluster() : CGraphItem()
{
}

CCluster::CCluster(IGraph * graph, ICluster * parent) : CGraphItem(graph), m_parent(parent)
{
	assert(m_parent);
	m_parent->AppendCluster(this);
}

void CCluster::Clear()
{
	CGraphItem::Clear();
	m_clusters.clear();
	m_vertices.clear();
}

//  ===  ICluster  ===
ICluster * CCluster::GetParent() const
{
	return m_parent;
}

const IClusterSet & CCluster::GetClusters() const
{
	return m_clusters;
}

const IVertexSet & CCluster::GetVertices() const
{
	return m_vertices;
}

void CCluster::AppendCluster(ICluster * cluster)
{
	m_clusters.insert(cluster);
}

void CCluster::RemoveCluster(ICluster * cluster)
{
	IClusterSet::const_iterator found = m_clusters.find(cluster);
	if (found != m_clusters.end())
		m_clusters.erase(found);
}

void CCluster::AppendVertex(IVertex * vertex)
{
	m_vertices.insert(vertex);
}

void CCluster::Walk(IClusterVisitor * visitor)
{
	for(IClusterSet::const_iterator itr = m_clusters.begin(); itr != m_clusters.end(); ++itr)
	{
		visitor->ItemVisited(*itr);
		itr->get()->Walk(visitor);
	}
}

void CCluster::Walk(IVertexVisitor * visitor)
{
	for(IVertexSet::const_iterator itr = m_vertices.begin(); itr != m_vertices.end(); ++itr)
	{
		visitor->ItemVisited(*itr);
	}
}

bool CCluster::OnlyConatinsOneCluster() const
{
	if (m_clusters.size() == 1 && m_vertices.size() == 0)
		return true;
	return false;
}

void CCluster::Delete()
{
	if (ICluster * parent = GetParent())
	{
		for (IClusterSet::const_iterator itr = m_clusters.begin(); itr != m_clusters.end(); ++itr)
			parent->AppendCluster(itr->get());
		for (IVertexSet::const_iterator itr = m_vertices.begin(); itr != m_vertices.end(); ++itr)
			parent->AppendVertex(itr->get());
		parent->RemoveCluster(this);
	}
}

}