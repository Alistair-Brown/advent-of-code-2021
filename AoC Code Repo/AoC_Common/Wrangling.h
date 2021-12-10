#pragma once
#include <list>

namespace Wrangling
{
	template <typename T>
	void InsertIntoOrderedList(std::list<T> &listToInsertInto, T valueToInsert)
	{
		if (listToInsertInto.size() == 0)
		{
			listToInsertInto.push_back(valueToInsert);
		}
		else
		{
			bool inserted{ false };
			for (typename std::list<T>::iterator iter = listToInsertInto.begin();
				iter != listToInsertInto.end();
				iter++)
			{
				if (valueToInsert < *iter)
				{
					listToInsertInto.insert(iter, valueToInsert);
					inserted = true;
					break;
				}
			}
			if (!inserted) { listToInsertInto.push_back(valueToInsert); }
		}
	}
}