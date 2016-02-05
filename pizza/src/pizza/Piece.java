package pizza;
/*
 * Represents a piece of pizza with a location and a shape
 */
class Piece implements Comparable<Piece> {
	Shape shape;
	int row, col;
	int hamCells;
	double value;
	double quality;

	Piece(Shape f, int row, int col, int hamCells) {
		shape = f;
		this.row = row;
		this.col = col;
		this.hamCells = hamCells;
	}

	/*
	 * sorts descending
	 * 
	 * @see java.lang.Comparable#compareTo(java.lang.Object)
	 */
	@Override
	public int compareTo(Piece o) {
		return (int) Math.signum(o.quality - quality);
	}

	@Override
	public String toString() {
		return row + " " + col + " " + (row + shape.rows - 1) + " " + (col + shape.cols - 1);
	}
}