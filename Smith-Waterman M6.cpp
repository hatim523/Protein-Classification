#include<iostream>
#include<string>
#include<string.h>
#include<fstream>

using namespace std;

/*************************************************************************************************/
/* Note: Please use Microsoft Visual Studio (VS) to run the code to avoid file read errors.		 */
/*************************************************************************************************/

int** Find3BestScores(int *data, int size)		//returns Index of 3 Max Numbers
{
	int **max = new int*[3];
	for (int i = 0; i < 3; i++)
	{
		max[i] = new int[2];
		memset(max[i], -214648, sizeof(int) * 2);
	}

	int *temp = new int[size];
	memcpy(temp, data, sizeof(int) * size);

	//finding top 3 scores
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (max[i][0] < temp[j])
			{
				max[i][0] = temp[j];
				max[i][1] = j;
			}
		}
		//reducing max score to minimun
		temp[max[i][1]] = -214648;
	}
	return max;
}

class Sequencing
{
private:
	string data1, data2;
	int size1, size2;
	int **matrix;
	string formed_s1, formed_s2;
	int gap, mismatch, match;
public:
	Sequencing(string _data1 = "\0", string _data2 = "\0")
	{
		data1 = _data1;
		data2 = _data2;

		formed_s1 = "";
		formed_s2 = "";

		size1 = data1.size();
		size2 = data2.size();

		gap = -10;
		mismatch = -18;
		match = 40;

		if (size1 > 0 && size2 > 0)		//Horizontal: data1   Vertical: data2
		{
			matrix = new int*[size2 + 1];
			for (int i = 0; i < size2 + 1; i++)
			{
				matrix[i] = new int[size1 + 1];
				memset(matrix[i], 0, sizeof(int) * (size1 + 1));
			}
		}
	}
	void RenewMatrix()
	{
		//delete matrix
		if (matrix != 0)
		{
			delete[] matrix;
		}
		//allocate memory for new matrix
		matrix = new int*[size2 + 1];
		for (int i = 0; i < size2 + 1; i++)
		{
			matrix[i] = new int[size1 + 1];
			memset(matrix[i], 0, sizeof(int) * (size1 + 1));
		}
	}
	int Match(int row, int col)
	{
		if (data2[row] == data1[col])
			return match;
		else
			return mismatch;
	}
	int TopLeft(int row, int col)	//checks for match
	{
		return matrix[row][col] + Match(row, col);
	}
	int Top(int row, int col)
	{
		return matrix[row][col] + gap;
	}
	int Left(int row, int col)
	{
		return matrix[row][col] + gap;
	}
	void SmithWaterman()	//Penalty on Mismatch: -1 //Reward on Match: +1
	{
		//initializing first row and first column
		int totalrows = size2 + 1;
		int totalcols = size1 + 1;
		//initializing first row
		for (int j = 1; j < totalcols; j++)
			matrix[0][j] = 0;
		//initializing first column
		for (int i = 1; i < totalrows; i++)
			matrix[i][0] = 0;

		//Matrix forming for choosing best allignment (Dynamic Programming)
		//columns will first first
		for (int i = 1; i < totalrows; i++)
		{
			for (int j = 1; j < totalcols; j++)
			{
				matrix[i][j] = max(TopLeft(i - 1, j - 1), Top(i - 1, j), Left(i, j - 1));
			}
			//Display();
		}

	}
	int max(int num1, int num2, int num3)
	{
		if (num1 > num2 && num1 >= num3)
			return num1;
		else if (num2 > num3 && num2 >= num1)
			return num2;
		else
			return num3;
	}
	void Display()
	{
		int l = 0;
		for (int i = 0; i < size2 + 1; i++)
		{
			if (i == 0)
			{
				cout << "\t";
				for (int k = 0; k < size1; k++)
					cout << "\t" << data1[k];
				cout << endl;
			}
			if (i > 0)
				cout << data2[l++] << "\t";
			else
				cout << "\t";
			for (int j = 0; j < size1 + 1; j++)
			{
				cout << matrix[i][j] << "\t";
			}
			cout << endl;
		}
	}
	void Input()
	{
		getline(cin, data1);
		size1 = data1.length();
	}
	void Input(string pdata)
	{
		data1 = pdata;
		size1 = data1.length();
	}
	int ClassInput1(string pdata)
	{
		data2 = pdata;
		size2 = data2.length();

		RenewMatrix();
		SmithWaterman();
		FormString();
		return matrix[size2][size1];
	}
	void FormString()
	{
		string s1 = "";
		string s2 = "";

		int i = data2.length();
		int j = data1.length();

		while (i > 0 && j > 0)
		{
			if (i > 0 && j > 0 && matrix[i][j] == matrix[i - 1][j - 1] + Match(i - 1, j - 1))
			{
				s2 += data2[--i];
				s1 += data1[--j];
			}
			else if (i > 0 && (matrix[i - 1][j] + gap) == matrix[i][j])
			{
				s2 += data2[--i];
				s1 += "-";
			}
			else
			{
				s1 += data1[--j];
				s2 += "-";
			}
		}

		for (int i = s1.length() - 1, j = 0; i >= 0; i--, j++)
			formed_s1 += s1[i];

		for (int i = s2.length() - 1, j = 0; i >= 0; i--, j++)
			formed_s2 += s2[i];

		//cout << formed_s1 << endl << formed_s2 << endl;
		//cout << "Score: " << matrix[size2][size1] << endl;
	}
};

int main()
{
	ifstream in("input2.csv");
	int correctans = 0;

	for (int l = 0; l < 10; l++)
	{
		string inputstring;
		getline(in, inputstring, '\n');
		int totalClasses = 10;
		int SubClasses = 8;

		//cout << inputstring << endl;

		Sequencing s1;
		//cout << "Enter Input Sequence: ";
		s1.Input(inputstring);

		int scores[10];
		memset(scores, 0, sizeof(int) * 10);
		int k = 0;
		//now comparing with all classes
		string filename = "class0.csv";

		for (int i = 0; i < totalClasses; i++)
		{
			filename[5] = '0' + i;
			ifstream read(filename);

			//bring pointer to sequence line
			string noimp, classstr;
			getline(read, noimp, ',');
			getline(read, noimp, ',');
			getline(read, noimp, '\n');

			int sum = 0;
			for (int j = 0; j < SubClasses; j++)
			{
				getline(read, noimp, ',');
				getline(read, noimp, ',');
				getline(read, classstr, '\n');
				sum += s1.ClassInput1(classstr);
			}
			scores[k++] = sum / SubClasses;
			read.close();
		}

		cout << "For Class " << l << endl;
		int **avg_score = Find3BestScores(scores, 10);
		cout << "Score: " << avg_score[0][0] << "\tClass: " << avg_score[0][1] << endl;
		if (l == avg_score[0][1])
			correctans++;

		cout << endl;
	}
	in.close();

	cout << "Correct Answers: " << correctans << endl;
	return 0;
}