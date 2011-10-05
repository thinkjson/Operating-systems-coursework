
public class MatrixMultiplier extends Thread {
	
	Matrix matrix;
	int size;

	public MatrixMultiplier(Matrix matrix) {
		this.matrix = matrix;
		this.size = this.matrix.getSize();
		
		int row, cols, z;
		long[] result;
		while (this.matrix.getQueueSize() > 0) {
			row = this.matrix.take();
			result = new long[this.size];
		    for (cols = 0; cols < this.size; cols++) {
		        for (z = 0; z < this.size; z++) {
		            result[z] += (long) this.matrix.getMatrix1()[row][cols] * 
		            	(long) this.matrix.getMatrix2()[cols][z];
		        }
		    }
		    
		    this.matrix.setResultRow(row, result);
		}
	}
}
