#include "DisplayView.h"

void DisplayView::Update(const std::vector<Tuple>& tuples) const
{
	for (const auto& tuple : tuples) 
	{
		PrintTuple(tuple);
	}
}