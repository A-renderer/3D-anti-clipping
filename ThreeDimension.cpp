#include "ThreeDimension.h"

ThreeDimension::ThreeDimension(){
	frontside = Polygon();
	backside = Polygon();
}

ThreeDimension::ThreeDimension(vector<Point> pol) {
	frontside = Polygon(pol);
	frontside.moveUp(10);
	frontside.moveRight(10);

	backside = Polygon(pol);

	for (int i=0; i<pol.size()-1; i++) {
		lines.push_back(Line(frontside.e[i], backside.e[i]));

		vector<Point> temp;
		if (i==pol.size()-1) {
			temp.push_back(frontside.e[i]);
			temp.push_back(frontside.e[0]);
			temp.push_back(backside.e[0]);
			temp.push_back(backside.e[i]);
		} else {
			temp.push_back(frontside.e[i]);
			temp.push_back(frontside.e[i+1]);
			temp.push_back(backside.e[i+1]);
			temp.push_back(backside.e[i]);
		}
		p.push_back(Polygon(temp));
	}
}

ThreeDimension::ThreeDimension(const ThreeDimension& ob) {
	frontside = ob.frontside;
	backside = ob.backside;
	lines = ob.lines;
	p = ob.p;
}

ThreeDimension& ThreeDimension::operator=(const ThreeDimension& ob) {
	frontside = ob.frontside;
	backside = ob.backside;
	lines = ob.lines;
	p = ob.p;

	return *this;
}

void ThreeDimension::moveLeft (float dx) {
	int bound = 10;
	if (frontside.getMinX() < bound && backside.getMinX() > bound){
		frontside.moveLeft(dx);
		backside.moveLeft(dx);
		for(int i = 0; i<lines.size(); i++){
			lines[i].moveLeft(dx);
			p[i].moveLeft(dx);
		}
	}
}

void ThreeDimension::moveRight (float dx) {
	int bound = 390;
	if (frontside.getMinX() > bound && backside.getMinX() < bound){
		frontside.moveRight(dx);
		backside.moveRight(dx);
		for(int i = 0; i<lines.size(); i++){
			lines[i].moveRight(dx);
			p[i].moveRight(dx);
		}
	}
}

void ThreeDimension::moveUp (float dy) {
	int bound = 10;
	if (frontside.getMinY() > bound && backside.getMinY() < bound){
		frontside.moveUp(dy);
		backside.moveUp(dy);
		for(int i = 0; i<lines.size(); i++){
			lines[i].moveUp(dy);
			p[i].moveUp(dy);
		}
	}
}

void ThreeDimension::moveDown (float dy) {
	int bound = 489;
	if (frontside.getMinY() > bound && backside.getMinY() < bound){
		frontside.moveDown(dy);
		backside.moveDown(dy);
		for(int i = 0; i<lines.size(); i++){
			lines[i].moveDown(dy);
			p[i].moveDown(dy);
		}
	}
}

void ThreeDimension::scale (float k) {
	frontside.scale(k);
	backside.scale(k);

	p.clear();
	lines.clear();

	for (int i=0; i<frontside.e.size()-1; i++) {
		lines.push_back(Line(frontside.e[i], backside.e[i]));

		vector<Point> temp;
		if (i==frontside.e.size()-1) {
			temp.push_back(frontside.e[i]);
			temp.push_back(frontside.e[0]);
			temp.push_back(backside.e[0]);
			temp.push_back(backside.e[i]);
		} else {
			temp.push_back(frontside.e[i]);
			temp.push_back(frontside.e[i+1]);
			temp.push_back(backside.e[i+1]);
			temp.push_back(backside.e[i]);
		}
		p.push_back(Polygon(temp));
	}
}

/*void ThreeDimension::scanLine(){
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
}*/