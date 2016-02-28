#include "Polygon.h"
#include "Line.h"
#include "ThreeDimension.h"

class FrameBuffer {
public:

	FrameBuffer() {
		fbfd = open("/dev/fb0", O_RDWR);
	    if (fbfd == -1) {
	        perror("Error: cannot open framebuffer device");
	        exit(1);
	    }
	    printf("The framebuffer device was opened successfully.\n");

	    // Get fixed screen information
	    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
	        perror("Error reading fixed information");
	        exit(2);
	    }

	    // Get variable screen information
	    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
	        perror("Error reading variable information");
	        exit(3);
	    }

	    printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);

	    // Figure out the size of the screen in bytes
	    int screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

	    // Map the device to memory
	    fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
	    if (atoi (fbp) == -1) {
	        perror("Error: failed to map framebuffer device to memory");
	        exit(4);
	    }
	    printf("The framebuffer device was mapped to memory successfully.\n");
	}

	~FrameBuffer() {
		munmap(fbp, 0);
		close(fbfd);
	}

	fb_var_screeninfo getvinfo() {
		return vinfo;
	}

	fb_fix_screeninfo getfinfo() {
		return finfo;
	}

	int getfbfd() {
		return fbfd;
	}

	char* getfbp() {
		return fbp;
	}

	void putPixel(Point P, int r, int g, int b, int a) {
		location = (P.x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                           (P.y+vinfo.yoffset) * finfo.line_length;

		*(fbp + location) = b; // blue 
        *(fbp + location + 1) = g;  // green
        *(fbp + location + 2) = r; // red
        *(fbp + location + 3) = a; // transparency
	}

	void drawLine(Point P1, Point P2, int r, int g, int b, int a) {
		int dx =  abs(P2.x-P1.x), sx = P1.x<P2.x ? 1 : -1; //sign value for x
	   	int dy = -abs(P2.y-P1.y), sy = P1.y<P2.y ? 1 : -1;  //sign value for y
	   	int err = dx+dy; int e2; /* error value e_xy */
	 
	   	for(;;){  /* loop */
	   		Point Ptemp(P1.x,P1.y);
	    	putPixel(Ptemp,r,g,b,a);

	      	if (P1.x==P2.x && P1.y==P2.y) break; //berarti titik berhimpit
	      	e2 = 2*err;
	      	if (e2 >= dy) { err += dy; P1.x += sx; } /* e_xy+e_x > 0 */
	      	if (e2 <= dx) { err += dx; P1.y += sy; } /* e_xy+e_y < 0 */
	   	}
	}

	void drawPolygon(Polygon P, int r, int g, int b, int a) {
		for (int i=0; i<P.n-1; ++i) {
			drawLine(P.e[i], P.e[i+1], r, g, b, a);
		}
		drawLine(P.e[P.n-1], P.e[0], r, g, b, a);
	}

    Polygon drawCircle(Point mid, int radius, int r, int g, int b, int t) {
	  	int x = radius;
		int y = 0;
		int decisionOver2 = 1-x;
		vector<Point> e; 

		while(x >= y) {
			putPixel(Point(x + mid.x, y + mid.y), r,g,b,t);
			e.push_back(Point(x + mid.x, y + mid.y));
			putPixel(Point(y + mid.x, x + mid.y), r,g,b,t); 
			e.push_back(Point(y + mid.x, x + mid.y));
			putPixel(Point(-x + mid.x, y + mid.y), r,g,b,t);  
			e.push_back(Point(-x + mid.x, y + mid.y));
			putPixel(Point(-y + mid.x, x + mid.y), r,g,b,t);  
			e.push_back(Point(-y + mid.x, x + mid.y));
			putPixel(Point(-x + mid.x, -y + mid.y), r,g,b,t);  
			e.push_back(Point(-x + mid.x, -y + mid.y));
			putPixel(Point(-y + mid.x, -x + mid.y), r,g,b,t);  
			e.push_back(Point(-y + mid.x, -x + mid.y));
			putPixel(Point(x + mid.x, -y + mid.y), r,g,b,t);  
			e.push_back(Point(x + mid.x, -y + mid.y));
			putPixel(Point(y + mid.x, -x + mid.y), r,g,b,t);  
			e.push_back(Point(y + mid.x, -x + mid.y));
			y++;
			if (decisionOver2<0) {
				decisionOver2 += 2 * y + 1;
			}
			else {
				x--;
				decisionOver2 += 2 * (y - x) + 1;
			}
		}

		Polygon pol(e);
		pol.firePoint = Point(mid.x, mid.y);
		floodFill(mid.x, mid.y,0,0,0,r,g,b);

		return pol;
	}

	int getB(int x, int y){
		int blue;
		int color = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                           (y+vinfo.yoffset) * finfo.line_length;
		blue = *(fbp + color); 
		if(blue < 0) {
			blue+=256;
		}
		return blue;
	}

	int getG(int x, int y){
		int green;
		int color = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                           (y+vinfo.yoffset) * finfo.line_length;
		green = *(fbp + color + 1); 
		if(green < 0){
			green+=256;
		}	
		return green;	
	}	

	int getR(int x, int y){
		int red;
		int color = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                           (y+vinfo.yoffset) * finfo.line_length;
		red = *(fbp + color + 2); 
		if(red < 0){
			red+=256;
		}
		return red;
	}		
		
	int getA(int x, int y){
		int trans;
		int color = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                           (y+vinfo.yoffset) * finfo.line_length;
		trans = *(fbp + color + 3); 
		if(trans < 0){
			trans+=256;
		}
		return trans;
	}

	void floodFill(int x, int y, int r_target, int g_target, int b_target, int r_rep, int g_rep, int b_rep) {
		if (r_target!=r_rep || g_target!=g_rep || b_target!=b_rep){
			// Get the pixel's color
			int r_node = getR(x,y);
			int g_node = getG(x,y);
			int b_node = getB(x,y);

			if(r_node==r_target && g_node==g_target && b_node==b_target)
			{
				// Fill the pixel with the replacement color
				putPixel(Point(x,y), r_rep, g_rep, b_rep, 0);

				floodFill(x+1,y, r_target, g_target, b_target, r_rep, g_rep, b_rep); //east
				floodFill(x-1,y, r_target, g_target, b_target, r_rep, g_rep, b_rep); //west
				floodFill(x, y+1, r_target, g_target, b_target, r_rep, g_rep, b_rep); //north
				floodFill(x,y-1, r_target, g_target, b_target, r_rep, g_rep, b_rep); //south
			}
		}
	}

	void clearscreen(){
	    int x,y;
	    for (x=0; x<1366; x++){
	        for (y=0; y<512; y++){
	            location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
	                           (y+vinfo.yoffset) * finfo.line_length;
	            *(fbp + location) = 0;        // blue
	            *(fbp + location + 1) = 0;      // green
	            *(fbp + location + 2) = 0;      // red
	            *(fbp + location + 3) = 0;      // transparency
	        }
	    }
	    // Sky blue : 135 206 235
	}

	void cleararea(int startX, int startY, int finishX, int finishY){
	    int x,y;
	    for (x=startX; x<finishX; x++){
	        for (y=startY; y<finishY; y++){
	            location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
	                           (y+vinfo.yoffset) * finfo.line_length;
	            *(fbp + location) = 235;        // blue
	            *(fbp + location + 1) = 206;      // green
	            *(fbp + location + 2) = 135;      // red
	            *(fbp + location + 3) = 0;      // transparency
	        }
	    }
	}
	void rasterScan(Polygon pol, int r, int g, int b, int a) {
		drawPolygon(pol, r, g, b, a);

		int n = pol.n;

		float slope[n];
		int line[n];
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

		vector<Line> lines;
		for (int y=pol.getMinY(); y<pol.getMaxY(); y++) {
			int k = 0;
			// Cari titik perpotongan
			for (int i=0; i<n; i++) {
				if (pol.e[i].y <= y && pol.e[i+1].y > y || pol.e[i+1].y <= y && pol.e[i].y > y) {
					if (!(pol.e[i-1].y < pol.e[i].y && pol.e[i+1].y < pol.e[i].y) || !(pol.e[i-1].y > pol.e[i].y && pol.e[i+1].y > pol.e[i].y)){
						line[k] = (int) (pol.e[i].x + slope[i] * (y - pol.e[i].y));
						k++;
					}
				}
			}

			//ngurutin line
			for (int j=0; j<k-1; j++) {
				for (int i=0; i<k-1; i++) {
					if (line[i] > line[i+1]) {
						int temp = line[i];
						line[i] = line[i+1];
						line[i+1] = temp;
					}
				}
			}


			for (int i=0; i<k; i+=2) {
				drawLine(Point(line[i], y), Point(line[i+1], y), r, g, b, a);
			}
		}
	}

	/*void drawWindow(Window w, int r, int g, int b, int t){
		drawPolygon(w.square,r,g,b,t);
	}

	void drawView(View v, int r, int g, int b, int t) {
		if(!v.lines.empty()) {
			for(int i=0; i<v.lines.size(); i++) {
				drawLine(v.lines[i].src, v.lines[i].dest, r, g, b, t);
			}	
		}
	}*/

	void draw3D(ThreeDimension obj, int r, int g, int b, int t){
		drawPolygon(obj.backside,r,g,b,t);
		drawPolygon(obj.frontside,r,g,b,t);
	}


	void scanLine3D(ThreeDimension obj, int r, int g, int b, int a){
		// initialize bucket
		map<int, vector<int> > buckets;
		for (int i=TOP; i<=BOTTOM; i++){
			vector<int> bucket;
			bucket.push_back(LEFT);
			bucket.push_back(RIGHT);
			buckets[i] = bucket;
		}


		// WARNAIN FRONT
		int n = obj.frontside.n;

		float slope[n];
		int* line;
		int x,y;
		for (int i=0; i<n; i++) {
			int dx = obj.frontside.e[i+1].x - obj.frontside.e[i].x;
			int dy = obj.frontside.e[i+1].y - obj.frontside.e[i].y;

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

		for (int y=obj.frontside.getMinY(); y<obj.frontside.getMaxY(); y++) {
			// Cari titik perpotongan dan urutkan
			int k=0;
			int line[obj.frontside.n];
			for (int i=0; i<obj.frontside.n; i++) {
				if (obj.frontside.e[i].y <= y && obj.frontside.e[i+1].y > y || obj.frontside.e[i+1].y <= y && obj.frontside.e[i].y > y) {
					if (!(obj.frontside.e[i-1].y < obj.frontside.e[i].y && obj.frontside.e[i+1].y < obj.frontside.e[i].y) || !(obj.frontside.e[i-1].y > obj.frontside.e[i].y && obj.frontside.e[i+1].y > obj.frontside.e[i].y)){
						line[k] = (int) (obj.frontside.e[i].x + slope[i] * (y - obj.frontside.e[i].y));
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


			for (int i=0; i<k; i+=2) {
				if(buckets.at(y).empty()) break;
				
				for(int j=0;j<buckets.at(y).size();j+=2) {
					int p = buckets.at(y).at(j);
					int q = buckets.at(y).at(j+1);

					if(line[i] < p) {
						if(line[i+1] < p) {
							// do nothing karena yang mau di gambar ada di sebelah kiri bucket
							// urutan : line[i]	line[i+1]	p 	q
						}
						else if(line[i+1] < q) {
							
							drawLine(Point(p, y), Point(line[i+1], y), r, g, b, a);
							// urutan : line[i]	p 	line[i+1] 	q
							buckets.at(y).at(j) = line[i+1];
						}
						else {
							drawLine(Point(p, y), Point(q, y), r, g, b, a);
							// urutan : line[i] p 	q 	line[i+1]
							buckets.at(y).erase(buckets.at(y).begin()+j); buckets.at(y).erase(buckets.at(y).begin()+j+1);
							j-=2; // ini bisa ga ya?
						}
					}
					else { // line[i] >= q
						if(line[i] > q) {
							// do nothing karena yang mau di gambar ada di sebelah kanan bucket
							// urutan : p 	q 	line[i]	line[i+1]
						}
						else if(line[i+1] < q) {
							drawLine(Point(line[i], y), Point(line[i+1], y), r, g, b, a);
							// urutan : p 	line[i]	line[i+1] 	q
							buckets.at(y).at(j+1) = line[i];
							buckets.at(y).push_back(line[i+1]);
							buckets.at(y).push_back(q);
							break; // karena udah kegambar semua, di break aja
						}
						else {
							drawLine(Point(line[i], y), Point(q, y), r, g, b, a);
							// urutan : p 	line[i]	q 	line[i+1]
							buckets.at(y).at(j+1) = line[i];
						}
					}
				}

			}
		}


		// WARNAIN BACK

		n = obj.backside.n;

		for (int i=0; i<n; i++) {
			int dx = obj.backside.e[i+1].x - obj.backside.e[i].x;
			int dy = obj.backside.e[i+1].y - obj.backside.e[i].y;

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

		for (int y=obj.backside.getMinY(); y<obj.backside.getMaxY(); y++) {
			// Cari titik perpotongan dan urutkan
			int k=0;
			int line[obj.backside.n];
			for (int i=0; i<obj.backside.n; i++) {
				if (obj.backside.e[i].y <= y && obj.backside.e[i+1].y > y || obj.backside.e[i+1].y <= y && obj.backside.e[i].y > y) {
					if (!(obj.backside.e[i-1].y < obj.backside.e[i].y && obj.backside.e[i+1].y < obj.backside.e[i].y) || !(obj.backside.e[i-1].y > obj.backside.e[i].y && obj.backside.e[i+1].y > obj.backside.e[i].y)){
						line[k] = (int) (obj.backside.e[i].x + slope[i] * (y - obj.backside.e[i].y));
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


			for (int i=0; i<k; i+=2) {
				if(buckets.at(y).empty()) break;
				
				for(int j=0;j<buckets.at(y).size();j+=2) {
					int p = buckets.at(y).at(j);
					int q = buckets.at(y).at(j+1);

					if(line[i] < p) {
						if(line[i+1] < p) {
							// do nothing karena yang mau di gambar ada di sebelah kiri bucket
							// urutan : line[i]	line[i+1]	p 	q
						}
						else if(line[i+1] < q) {
							
							drawLine(Point(p, y), Point(line[i+1], y), r, g+100, b, a);
							// urutan : line[i]	p 	line[i+1] 	q
							buckets.at(y).at(j) = line[i+1];
						}
						else {
							drawLine(Point(p, y), Point(q, y), r, g+100, b, a);
							// urutan : line[i] p 	q 	line[i+1]
							buckets.at(y).erase(buckets.at(y).begin()+j); buckets.at(y).erase(buckets.at(y).begin()+j+1);
							j-=2; // ini bisa ga ya?
						}
					}
					else { // line[i] >= q
						if(line[i] > q) {
							// do nothing karena yang mau di gambar ada di sebelah kanan bucket
							// urutan : p 	q 	line[i]	line[i+1]
						}
						else if(line[i+1] < q) {
							drawLine(Point(line[i], y), Point(line[i+1], y), r, g+100, b, a);
							// urutan : p 	line[i]	line[i+1] 	q
							buckets.at(y).at(j+1) = line[i];
							buckets.at(y).push_back(line[i+1]);
							buckets.at(y).push_back(q);
							break; // karena udah kegambar semua, di break aja
						}
						else {
							drawLine(Point(line[i], y), Point(q, y), r, g+100, b, a);
							// urutan : p 	line[i]	q 	line[i+1]
							buckets.at(y).at(j+1) = line[i];
						}
					}
				}

			}
		}

		for(int i=0; i<obj.p.size(); i++){
			drawLine(obj.lines[i].src, obj.lines[i].dest, r, g, b, a);
		}
	}

private:
	struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;
	int fbfd;					/* frame buffer file descriptor */
	char* fbp;
	long int screensize;
	long int location;
};
