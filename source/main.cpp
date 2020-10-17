#include "Parameters.cpp"

int main(int argc, char *const argv[])
{
	Parameters parameters(argc, argv);

	if (parameters.executeInThreads())
	{
		cerr << "End of Execution" << endl;
	}
    else
    {
        cerr << "Wrong Parameters" << endl;
    }
	return 0;
}