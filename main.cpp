#include "main.hpp"

int main()
{
	IOer1.Follow(Outputer2._OutletAdr,1,1,3);
	IOer2.Follow(Outputer1._OutletAdr,1,1,2);

	IOer3.Follow(IOer1._OutletAdr,1,1,3);
	IOer3.Follow(IOer2._OutletAdr,4,1,2);

	Printer(1,IOer3._OutletAdr);

	TopSec();
}
