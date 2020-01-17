///////////////////////////////////////////////////////////////////////////////////////////////////
// utilsPatternCommand.h
//
// Standard ISO/IEC 114882, C++14
//
// |   version  |    release    | Description
// |------------|---------------|---------------------------------
// |      1     |   2020 01 06  |
// |            |               | 
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <list>
#include <type_traits>

namespace utils
{

namespace pattern_Command
{

class tObjectCommand
{
protected:
	class tCommand
	{
		tObjectCommand* m_pObj = nullptr;

		tCommand() { };

	public:
		explicit tCommand(tObjectCommand* obj) :m_pObj(obj) { }
		virtual ~tCommand() { }

		template <class T>
		T* GetObject() const
		{
			if (std::is_same<tObjectCommand, T>::value || std::is_base_of<tObjectCommand, T>::value)
			{
				return reinterpret_cast<T*>(m_pObj);
			}

			return nullptr;
		}

		virtual void operator()() = 0;
	};

private:
	std::list<tCommand*> m_CommandList;

	const std::size_t m_CommandListSize = 10;

	tCommand* m_CommandActive = nullptr;

public:
	virtual ~tObjectCommand()
	{
		delete m_CommandActive;

		for (tCommand* i : m_CommandList)
		{
			delete i;
		}
	}

protected:
	tCommand* GetCommand()
	{
		if (m_CommandActive == nullptr && m_CommandList.size() > 0)
		{
			m_CommandActive = m_CommandList.front();
			m_CommandList.pop_front();
		}

		return m_CommandActive;
	}

	tCommand* SetCommandNext()
	{
		delete m_CommandActive;

		m_CommandActive = nullptr;

		return GetCommand();
	}

	bool PutCommand(tCommand* cmd)
	{
		if (m_CommandList.size() < m_CommandListSize)
		{
			m_CommandList.push_back(cmd);

			return true;
		}

		return  false;
	}
};

}

}
