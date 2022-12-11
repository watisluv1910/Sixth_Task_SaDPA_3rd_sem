#include <iostream>
#include <vector>
#include <stack>
#include <cmath>
#include <windows.h>
#include <locale>
#include <cstdlib>

using std::cin;
using std::cout;
using std::vector;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

size_t find_min(const size_t& a, const size_t& b);
void highlightSubmatrix(vector<vector<size_t>>, size_t, size_t, size_t, size_t);
void findSubmatrixes(vector<vector<size_t>>, size_t);
void findSubmatrixesOptimized(vector<vector<size_t>>&, size_t);
void printMatrix(vector<vector<size_t>>);

int main()
{
    srand(time(0));
    SetConsoleTextAttribute(console, 7);
    size_t height, width, search_width, i, j, x, y;
	
    cout << "Enter the matrix parameters:\n";
    cout << "Height = ";
    cin >> height;
    cout << "Width = ";
    cin >> width;	
	vector<vector<size_t>> matrix(height, vector<size_t>(width));
	
	// Generate random matrix
	for (vector<size_t>& row : matrix)
		for (size_t& elem : row)
			elem = rand() % 2;
	
    cout << "\nEnter square search-submatrix width:\n";
    cin >> search_width;
	
    if (search_width > find_min(height, width) || !search_width)
    {
		std::cerr << "Error. Search-submatrix has a wrong size.\n";
        return 1;
    }
	
	findSubmatrixes(matrix, search_width);
	findSubmatrixesOptimized(matrix, search_width);

    system("pause");
    return 0;
}

// find minimum of two integers
size_t find_min(const size_t& a, const size_t& b)
{
	return (a < b) ? a : b;
}

// find maximum of two integers
size_t find_max(const size_t& a, const size_t& b)
{
	return (a > b) ? a : b;
}

void findSubmatrixes(vector<vector<size_t>> matrix, size_t search_width)
{
	size_t x, y, i, j, search_size;

	size_t width = matrix.at(0).size();
	size_t height = matrix.size();
	x = width - search_width;
	y = height - search_width;

	size_t iter_counter = 0;

	cout << "\nFirst method results:\n";
	
	for (size_t b = 0; b < y + 1; b++) // 
	{
		for (size_t a = 0; a < x + 1; a++)
		{
			search_size = 0; // Search matrix size
			for (i = b; i < search_width + b; i++)
			{
				for (j = a; j < search_width + a; j++)
				{
					if (!matrix.at(i).at(j))
					{
						search_size++;
					}
					iter_counter++;
				}
				iter_counter++;
			}
			if (search_size == (int)pow(search_width, 2)) // Searched zeros matrix is a square
			{
				// Submatrix was found and it's coordinates are (a, b) and (j - 1, i - 1)
				cout << "\nSubmatrix was found and it's coordinates are (" 
					<< a << ", " << b << ") and (" 
					<< j - 1 << ", " << i - 1 << ").\n";

				cout << "Number of iterations: " << iter_counter << "\n";
				return;
			}
			iter_counter++;
		}
		iter_counter++;
	}
	
	cout << "\nSubmatrix was not found.\n";
	cout << "Number of iterations: " << iter_counter << "\n";
}

// Use branch and bound method to find submatrix
void findSubmatrixesOptimized(vector<vector<size_t>> &matrix, size_t search_width)
{
	size_t rows = matrix.size(), 
		cols = matrix.at(0).size(),
		iter_counter = 0;
	
	vector<vector<size_t>> horizontal_submatrices = vector<vector<size_t>>(rows, vector<size_t>(cols, 0));

	cout << "\nSecond method results:\n";
	
	for (int i = 0; i < rows; i++) 
	{
		for (int j = cols - 1; j >= 0; j--) 
		{
			if (matrix[i][j] == 1)
			{
				continue;
			}

			horizontal_submatrices.at(i).at(j) += !matrix.at(i).at(j);

			if (j != cols - 1)
			{
				horizontal_submatrices.at(i).at(j) += horizontal_submatrices.at(i).at(j + 1);
			}

			iter_counter++;
		}		

		if (i >= search_width - 1)
		{
			for (size_t j = 0; j < cols - search_width + 1; j++)
			{
				if (horizontal_submatrices.at(i).at(j) >= search_width)
				{
					for (size_t k = i; k >= (i + 1) - search_width; --k)
					{
						if (horizontal_submatrices.at(k).at(j) < search_width)
						{
							break;
						} 
						if (k == (i + 1) - search_width)
						{
							cout << "\nSubmatrix was found and it's coordinates are ("
								<< j << ", " << k << ") and ("
								<< j + (search_width - 1) << ", " << k + (search_width - 1) << ").\n";

							cout << "Number of iterations: " << iter_counter << "\n";
							return;
						}

						iter_counter++;
					}
				}

				iter_counter++;
			}
		}

		iter_counter++;
	}

	cout << "\nSubmatrix was not found.\n";
	cout << "Number of iterations: " << iter_counter << "\n";
}

//size_t width = matrix.at(0).size();
//size_t height = matrix.size();
//
//vector<int> d(width, -1), d1(width), d2(width); // 
//std::stack<int> stack;
//
//int answer = 0;
//size_t iter_counter = 0;
//
//for (int i = 0; i < height; i++)
//{
//	for (int j = 0; j < width; j++)
//	{
//		if (matrix.at(i).at(j))
//		{
//			d.at(j) = i; // The biggest row number with 1 in j-th column
//		}
//		iter_counter++;
//	}
//	while (!stack.empty())
//	{
//		stack.pop();
//		iter_counter++;
//	}
//	for (int j = 0; j < width; j++) {
//		while (!stack.empty() && d.at(stack.top()) <= d.at(j))
//		{
//			stack.pop();
//			iter_counter++;
//		}
//		d1[j] = stack.empty() ? -1 : stack.top();
//		stack.push(j);
//		iter_counter++;
//	}
//	while (!stack.empty())
//	{
//		stack.pop();
//		iter_counter++;
//	}
//	for (int j = width - 1; j >= 0; j--) {
//		while (!stack.empty() && d.at(stack.top()) <= d.at(j))
//		{
//			stack.pop();
//			iter_counter++;
//		}
//		d2[j] = stack.empty() ? width : stack.top();
//		stack.push(j);
//		iter_counter++;
//	}
//	for (int j = 0; j < width; j++)
//	{
//		answer = find_max(answer, (i - d[j]) * (d2[j] - d1[j] - 1));
//		iter_counter++;
//	}
//	iter_counter++;
//}
//cout << "\nSubmatrix number of elements " << answer << ".\n";
//cout << "Number of iterations: " << iter_counter << "\n";

void highlightSubmatrix(vector<vector<size_t>> matrix, 
	size_t a, size_t b, size_t i, size_t j)
{
	cout << '\n';
	for (size_t x = 0; x < matrix.size(); x++)
	{
		for (size_t y = 0; y < matrix.at(0).size(); y++)
		{
			if (((x >= b) && (x < i)) && ((y >= a) && (y < j)))
			{
				SetConsoleTextAttribute(console, 10);
				cout << matrix.at(x).at(y) << " ";
			}
			else
			{
				SetConsoleTextAttribute(console, 7);
				cout << matrix.at(x).at(y) << " ";
			}
		}
		cout << '\n';
	}
	SetConsoleTextAttribute(console, 7);
}

void printMatrix(vector<vector<size_t>> matrix)
{
	for (vector<size_t>& row : matrix)
	{
		for (size_t& elem : row)
		{
			cout << elem << " ";
		}
		cout << "\n";
	}
}