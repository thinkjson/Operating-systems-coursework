import java.util.Random;
import java.util.concurrent.ArrayBlockingQueue;

public class Matrix {
	
	private int size;
	private int nprocs;
	private Random prng;
	private int[][] matrix1, matrix2;
	private long[][] result;
	private MatrixMultiplier[] threads;
	private final ArrayBlockingQueue<Integer> threadQueue;
	
	public Matrix(String size) {
		this.size = Integer.parseInt(size);
		this.prng = new Random();
		this.threadQueue = new ArrayBlockingQueue<Integer>(this.size * this.size);
		this.populate();
		this.fillQueue();
		this.init_threads();
		
		System.out.printf("%15s%15d.0\t", "java threads", this.size);
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		Matrix matrix = new Matrix(args[0]);
	}

	private void populate() {
		this.matrix1 = new int[this.size][this.size];
		this.matrix2 = new int[this.size][this.size];
		this.result = new long[this.size][this.size];
		
		// Populate matrices
		for (int row = 0; row < this.size; row++) {
			for (int col = 0; col < this.size; col++) {
				this.matrix1[row][col] = this.prng.nextInt(Integer.MAX_VALUE);
				this.matrix2[row][col] = this.prng.nextInt(Integer.MAX_VALUE);
				this.result[row][col] = 0;
			}
		}
	}
	
	private void fillQueue() {
		// Fill queue with indices
		for (int i = 0; i < this.size; i++) {
			try {
				this.threadQueue.put(i);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		
		// Send shutdown signal
		for (int i = 0; i < nprocs; i++) {
			try {
				this.threadQueue.put(-1);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}
	
	public int take() {
		try {
			return this.threadQueue.take();
		} catch (InterruptedException e) {
			e.printStackTrace();
			return 0;
		}
	}
	
	private void init_threads() {
		this.nprocs = Runtime.getRuntime().availableProcessors();
		this.threads = new MatrixMultiplier[this.nprocs];
		
		for (int i = 0; i < nprocs; i++) {
			this.threads[i] = new MatrixMultiplier(this);
			this.threads[i].start();
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
	public void setResultRow(int row, long[] resultRow) {
		this.result[row] = resultRow;
	}

	/**
	 * @return the matrix size
	 */
	public int getSize() {
		return size;
	}
	
	/**
	 * @return the queue size
	 */
	public int getQueueSize() {
		return this.threadQueue.size();
	}
}
