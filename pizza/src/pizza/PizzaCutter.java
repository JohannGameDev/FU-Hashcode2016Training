package pizza;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.util.ArrayList;
import java.util.Collections;

public class PizzaCutter {

	static final String INPUT_FILE = "pizza.in";
	static final String OUTPUT_FILE = "pizza.out";
	
	/*
	 * Running time of the algorithm
	 * Adjust this value before lunch time
	 */
	static final long MAX_RUNNING_TIME = 2000000L; // 10 seconds

	/*
	 * weights the bulkiness against the other values in the value computation
	 */
	static final double BULKINESS_FACTOR = 0.3;

	/*
	 * adds randomness in the order the shapes are tried on the pizza
	 */
	static final double RANDOMNESS = 0.2;

	/*
	 * try pieces on edges first
	 */
	static final double EDGE_BONUS = 1.0;

	public static void main(String[] args) {
		PizzaCutter cutter = new PizzaCutter();
		cutter.cut();
	}

	/*
	 * Creates a solution for the pizza
	 */
	private void cut() {

		// record the start time
		long startTime = System.currentTimeMillis();
		
		// the best solution
		ArrayList<Piece> bestSolution = null;
		int bestNumberOfCells = -1;

		// make pizza
		Pizza freshPizza = Pizza.readFromFile(INPUT_FILE);

		// create shapes
		ArrayList<Shape> shapes = new ArrayList<>();
		for (int cols = 1; cols < freshPizza.maxCells; ++cols) {
			for (int rows = 1; rows < freshPizza.maxCells; ++rows) {
				if (cols * rows > freshPizza.maxCells) {
					break;
				}
				if (cols * rows >= freshPizza.minHamCells) {
					shapes.add(new Shape(rows, cols));
				}
			}
		}

		// create potential pieces.
		ArrayList<Piece> potentialPieces = new ArrayList<>();
		for (Shape shape : shapes) {
			for (int row = 0; row + shape.rows < freshPizza.rows; ++row) {
				for (int col = 0; col + shape.cols < freshPizza.cols; ++col) {
					int hamCells = freshPizza.countHamCells(shape, row, col);

					// check if there are enough ham cells
					if (hamCells >= freshPizza.minHamCells) {
						potentialPieces.add(new Piece(shape, row, col, hamCells));
					}
				}
			}
		}

		// run the solving algorithm for the specified time
		while (System.currentTimeMillis() < startTime + MAX_RUNNING_TIME) {

			// iterate on potential pieces and set their qualities
			// tweek here in order to improve the results
			for (Piece p : potentialPieces) {

				p.quality = (double) p.shape.size / p.hamCells;
				p.quality -= p.shape.bulkiness * BULKINESS_FACTOR;

				// check for horizontal edge
				if (p.row == 0 || (p.row + p.shape.rows == freshPizza.rows - 1)) {
					p.quality += EDGE_BONUS;
				}

				// check for vertical edge
				if (p.col == 0 || (p.col + p.shape.cols == freshPizza.cols - 1)) {
					p.quality += EDGE_BONUS;
				}
				p.quality += p.quality * Math.random() * RANDOMNESS;
			}

			// sort potentialPieces
			Collections.sort(potentialPieces);

			// initialize cutted pizza
			Pizza cutPizza = new Pizza(freshPizza.rows, freshPizza.cols, ' ');

			// iterate on all potentialPieces and collect actual pieces
			ArrayList<Piece> actualPieces = new ArrayList<>();
			for (Piece piece : potentialPieces) {

				// check if the piece does not intersect a previous one
				if (!cutPizza.intersects(piece)) {

					// add piece to the actual pieces
					actualPieces.add(piece);

					// cut the pizza
					for (int i = 0; i < piece.shape.cols; ++i) {
						boolean verticalEdge = i == 0 || (i == piece.shape.cols - 1);
						for (int j = 0; j < piece.shape.rows; ++j) {
							boolean horizontalEdge = j == 0 || (j == piece.shape.rows - 1);
							char c = '#';
							if (verticalEdge && horizontalEdge)
								c = '+';
							else if (verticalEdge)
								c = '|';
							else if (horizontalEdge)
								c = '-';
							cutPizza.setCell(piece.row + j, piece.col + i, c);
						}
					}
				}
			}

			// evaluate solution
			int numberOfCells = PizzaChecker.countCells(actualPieces);
			
			// check if this is the best solution so far
			if (numberOfCells > bestNumberOfCells) {
				
				// record solution
				bestNumberOfCells = numberOfCells;
				bestSolution = actualPieces;
				
				// check solution
				if (!PizzaChecker.checkSolution(freshPizza, bestSolution)) {
					System.out.println("Illegal solution. Exit Program");
					System.exit(0);
				}
				System.out.println("Number of Cells: " + numberOfCells);
			}
		}

		// output solution
		try {
			BufferedWriter writer = new BufferedWriter(new FileWriter(OUTPUT_FILE));
			writer.write(Integer.toString(bestSolution.size()));
			writer.newLine();
			for (Piece piece : bestSolution) {
				writer.write(piece.toString());
				writer.newLine();
			}
			writer.flush();
			writer.close();

		} catch (Exception e) {
			e.printStackTrace();
		}
		
		System.out.println("Solution written to " + OUTPUT_FILE);
	}
}
