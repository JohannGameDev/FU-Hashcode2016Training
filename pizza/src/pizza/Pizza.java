package pizza;

import java.io.BufferedReader;
import java.io.FileReader;
import java.util.StringTokenizer;

/*
 * Data class for Pizza
 */
public class Pizza {

	private char[][] cells;
	int rows;
	int cols;
	int minHamCells;
	int maxCells;

	/*
	 * creates a Pizza
	 */
	public Pizza(int rows, int cols) {
		this.rows = rows;
		this.cols = cols;
		cells = new char[rows][cols];
	}

	/*
	 * Creates a Pizza and initializes all cells with c
	 */
	public Pizza(int rows, int cols, char c) {
		this(rows, cols);
		for (int col = 0; col < cols; col++) {
			for (int row = 0; row < rows; row++) {
				cells[row][col] = c;
			}
		}
	}

	/*
	 * Gets the content of a single cell
	 */
	public char getCell(int row, int col) {
		return cells[row][col];
	}

	/*
	 * sets the content of a single cell
	 */
	public void setCell(int row, int col, char value) {
		cells[row][col] = value;
	}

	/*
	 * counts the number of ham cells contained in this piece
	 */
	public int countHamCells(Shape f, int row, int col) {
		int hamCells = 0;
		for (int i = 0; i < f.cols; ++i) {
			for (int j = 0; j < f.rows; ++j) {
				if (cells[row + j][col + i] == 'H') {
					hamCells += 1;
				}
			}
		}
		return hamCells;
	}

	/*
	 * Checks if the piece intersects any other piece of this pizza
	 */
	public boolean intersects(Piece piece) {
		for (int i = 0; i < piece.shape.cols; ++i) {
			for (int j = 0; j < piece.shape.rows; ++j) {
				if (cells[piece.row + j][piece.col + i] != ' ') {
					return true;
				}
			}
		}
		return false;
	}

	@Override
	public String toString() {
		StringBuilder sb = new StringBuilder();
		for (int row = 0; row < rows; ++row) {
			for (int col = 0; col < cols; ++col) {
				sb.append(cells[row][col]);
			}
			sb.append('\n');
		}
		return sb.toString();
	}

	public static final Pizza readFromFile(String file) {
		try {
			BufferedReader reader = new BufferedReader(new FileReader(file));

			// read first line
			StringTokenizer tt = new StringTokenizer(reader.readLine());
			int rows = Integer.parseInt(tt.nextToken());
			int cols = Integer.parseInt(tt.nextToken());
			Pizza pizza = new Pizza(rows, cols);

			pizza.minHamCells = Integer.parseInt(tt.nextToken());
			pizza.maxCells = Integer.parseInt(tt.nextToken());

			// read lines from file and set them as cells in the pizza
			for (int y = 0; y < rows; ++y) {
				char[] line = reader.readLine().toCharArray();
				for (int x = 0; x < cols; ++x) {
					pizza.setCell(y, x, line[x]);
				}
			}

			return pizza;
		} catch (Exception e) {
			e.printStackTrace();
		}
		return null; // parsing unsuccessful
	}
}
