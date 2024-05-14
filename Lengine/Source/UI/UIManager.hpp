#pragma once
#include <vector>
#include <UI/UILayer.hpp>
#include <Utility/Manager.hpp>

using std::vector;

class UIStack
{
public:

	void Push( UILayer* a_Layer ) { m_Layers.push_back( a_Layer ); }
	UILayer* Pop() { UILayer* Front = m_Layers.back(); m_Layers.pop_back(); return Front; }
	UILayer* Peek() const { return m_Layers.back(); }
	size_t Size() const { return m_Layers.size(); }
	void Clear() { m_Layers.clear(); }

private:

	vector< UILayer* > m_Layers;
};

class UIManager : Manager< UIManager >
{
	
};