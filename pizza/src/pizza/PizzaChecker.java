package pizza;

import java.util.BitSet;
import java.util.List;

public class PizzaChecker {

	public static void main(String[] args) {

		// read pizza

	}

	/*
	 * counts all cells covered by these pieces. Does NOT check if the solution
	 * is valid.
	 */
	public static int countCells(List<Piece> pieces) {
		int cellCounter = 0;
		for (Piece p : pieces) {
			cellCounter += p.shape.size;
		}
		return cellCounter;
	}

	public static boolean checkSolution(Pizza pizza, List<Piece> pieces) {

		// create a bitset
		BitSet bitSet = new BitSet(pizza.cols * pizza.rows);

		// iterate on pieces
		for (Piece p : pieces) {

			// catch IndexOutOfBoundsException to identify a bad solution
			try {

				// iterate on cells
				int hamCounter = 0;
				int cellCounter = 0;
				for (int i = 0; i < p.shape.rows; ++i) {
					int rowOffset = (p.row + i) * pizza.cols;
					for (int j = 0; j < p.shape.cols; ++j) {
						int bitIndex = rowOffset + p.col + j;
						// check if cell already marked
						if (bitSet.get(bitIndex)) {
							System.out.println("Bad solution, two pieces intersects each other");
							return false;
						}

						// otherwise mark the cell
						bitSet.set(bitIndex);

						// count ham and cells
						if (pizza.getCell(p.row + i, p.col + j) == 'H') {
							hamCounter++;
						}
						cellCounter++;
					}
				}

				// check counters
				if (hamCounter < pizza.minHamCells) {
					System.out.println("Bad solution. Only " + hamCounter + " in one piece.");
					return false;
				}
				if (cellCounter > pizza.maxCells) {
					System.out.println("Bad solution. One piece with " + cellCounter + " cells.");
					return false;
				}
			} catch (IndexOutOfBoundsException e) {
				System.out.println("Bad solution. One piece out of Pizza Bounds.");
				return false;
			}

		}
		return true;
	}

}
