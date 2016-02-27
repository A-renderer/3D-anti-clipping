#include "ThreeDimension.h"

ThreeDimension::ThreeDimension(){

}
void ThreeDimension::scanLine(){
	// The usual method starts with edges of projected polygons inserted into buckets, one per scanline; the rasterizer maintains an active edge table (AET). Entries maintain sort links, X coordinates, gradients, and references to the polygons they bound. To rasterize the next scanline, the edges no longer relevant are removed; new edges from the current scanlines' Y-bucket are added, inserted sorted by X coordinate. The active edge table entries have X and other parameter information incremented. Active edge table entries are maintained in an X-sorted list by bubble sort, effecting a change when 2 edges cross. After updating edges, the active edge table is traversed in X order to emit only the visible spans, maintaining a Z-sorted active Span table, inserting and deleting the surfaces when edges are crossed.

	int n = pol.n;

	// initialize bucket
	vector<int> bucket;
	bucket.push_back(LEFT);
	bucket.push_back(RIGHT);

	float slope[n];
	int* line;
	int x,y;
	for (int i=0; i<n; i++) {
		int dx = pol.e[i+1].x - pol.e[i].x;
		int dy = pol.e[i+1].y - pol.e[i].y;

		if (dy == 0) {
			slope[i] = 1;
		}
		if (dx == 0) {
			slope[i] = 0;
		}
		if (dx != 0 && dy != 0) {
			slope[i] = (float) dx/dy;
		}
	}

	for (int y=0; y<BOTTOM; y++) {
		// Cari titik perpotongan dan urutkan
		line = intersection(pol,y,slope);

		for (int i=0; i<k; i+=2) {
			if(bucket.empty()) break;
			
			for(int j=0;j<bucket.size();j+=2) {
				int a = bucket[j];
				int b = bucket[j+1];

				if(line[i] < a) {
					if(line[i+1] < a) {
						// do nothing karena yang mau di gambar ada di sebelah kiri bucket
						// urutan : line[i]	line[i+1]	a 	b
					}
					else if(line[i+1] < b) {
						// drawLine(Point(a, y), Point(line[i+1], y), r, g, b, a);
						// urutan : line[i]	a 	line[i+1] 	b
						bucket[j] = line[i+1];
					}
					else {
						// drawLine(Point(a, y), Point(b, y), r, g, b, a);
						// urutan : line[i] a 	b 	line[i+1]
						bucket.erase(j); bucket.erase(j+1);
						j-=2; // ini bisa ga ya?
					}
				}
				else { // line[i] >= a
					if(line[i] > b) {
						// do nothing karena yang mau di gambar ada di sebelah kanan bucket
						// urutan : a 	b 	line[i]	line[i+1]
					}
					else if(line[i+1] < b) {
						// drawLine(Point(line[i], y), Point(line[i+1], y), r, g, b, a);
						// urutan : a 	line[i]	line[i+1] 	b
						bucket[j+1] = line[i];
						bucket.push_back(line[i+1]);
						bucket.push_back(b);
						break; // karena udah kegambar semua, di break aja
					}
					else {
						// drawLine(Point(line[i], y), Point(b, y), r, g, b, a);
						// urutan : a 	line[i]	b 	line[i+1]
						bucket[j+1] = line[i];
					}
				}
			}

		}
	}
}

int* ThreeDimension::intersection(Polygon pol, int y, float slope[]) {
	int k=0;
	int line[pol.n];
	for (int i=0; i<pol.n; i++) {
		if (pol.e[i].y <= y && pol.e[i+1].y > y || pol.e[i+1].y <= y && pol.e[i].y > y) {
			if (!(pol.e[i-1].y < pol.e[i].y && pol.e[i+1].y < pol.e[i].y) || !(pol.e[i-1].y > pol.e[i].y && pol.e[i+1].y > pol.e[i].y)){
				line[k] = (int) (pol.e[i].x + slope[i] * (y - pol.e[i].y));
				k++;
			}
		}
	}

	// Sorting the lines
	for (int j=0; j<k-1; j++) {
		for (int i=0; i<k-1; i++) {
			if (line[i] > line[i+1]) {
				int temp = line[i];
				line[i] = line[i+1];
				line[i+1] = temp;
			}
		}
	}

	return line;
}