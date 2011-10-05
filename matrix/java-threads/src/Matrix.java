import java.util.Random;
import java.util.concurrent.ArrayBlockingQueue;

public class Matrix {
	
	private int size;
	private int nprocs;
	private Random prng;
	private int[][] matrix1, matrix2, result;
	private MatrixMultiplier[] threads;
	private final ArrayBlockingQueue<Integer> threadQueue;
	
	public Matrix(String size) {
		this.size = Integer.parseInt(size);
		this.prng = new Random();
		this.threadQueue = new ArrayBlockingQueue<Integer>(this.size * this.size);
		this.populate();
		this.init_threads();
		this.fillQueue();
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		@SuppressWarnings("unused")
		Matrix matrix = new Matrix(args[0]);
	}

	private void populate() {
		this.matrix1 = new int[this.size][this.size];
		this.matrix2 = new int[this.size][this.size];
		this.result = new int[this.size][this.size];
		
		// Populate matrices
		for (int row = 0; row < this.size; row++) {
			for (int col = 0; col < this.size; col++) {
				this.matrix1[row][col] = this.prng.nextInt();
				this.matrix2[row][col] = this.prng.nextInt();
				this.result[row][col] = 0;
			}
		}
	}
	
	private void fillQueue() {
		
	}
	
	private void init_threads() {
		this.nprocs = Runtime.getRuntime().availableProcessors();
		this.threads = new MatrixMultiplier[this.nprocs];
		
		for (int i = 0; i < nprocs; i++) {
			this.threads[i] = new MatrixMultiplier();
		}
	}

	/**
	 * @return the matrix1
	 */
	public int[][] getMatrix1() {
		return matrix1;
	}

	/**
	 * @return the matrix2
	 */
	public int[][] getMatrix2() {
		return matrix2;
	}

	/**
	 * @param result the result to set
	 */
	public void setResultRow(int row, int[] resultRow) {
		this.result[row] = resultRow;
	}
}
