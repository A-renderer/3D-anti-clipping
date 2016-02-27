#include "Polygon.h"

class ThreeDimension {
	public:
		Polygon frontside;
		Polygon backside;

		ThreeDimension();
		void scanLine();

		int* intersection(Polygon pol, int y, float slope[]);

}