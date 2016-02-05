package pizza;
/*
 * Represents a shape that a piece of pizza can have, in respect to the minimum
 * amount of ham and the maximum overall size. 
 */
class Shape {
	int cols;
	int rows;
	int size; 
	double bulkiness;

	public Shape(int rows, int cols) {
		this.cols = cols;
		this.rows = rows;
		
		//store values in as fields so you only need to compute them once
		this.size = cols * rows;
		this.bulkiness = Math.max((double) cols / rows, (double) rows / cols);
	}
}