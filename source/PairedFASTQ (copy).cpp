#include "SingleFASTQ.cpp"

class PairedFASTQ
{
	pair<SingleFASTQ, SingleFASTQ> pairedSequence;

public:
	void setPair(SingleFASTQ forward, SingleFASTQ reverse);
	pair<SingleFASTQ, SingleFASTQ> getPair();
};

void PairedFASTQ::setPair(SingleFASTQ forward, SingleFASTQ reverse)
{
	pairedSequence = make_pair(forward, reverse);
}

pair<SingleFASTQ, SingleFASTQ> PairedFASTQ::getPair()
{
	return pairedSequence;
}