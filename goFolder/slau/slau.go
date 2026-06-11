package slau

import (
	"fmt"
	"math"
	"strings"
)

type SeidelIteration struct {
	Iter int
	X    []float64
	Diff float64
}

func multiply(A, B [][]float64) [][]float64 {
	numberRowsA := len(A)
	numberRowsB := len(B[0])
	numberColsA := len(A[0])
	matrix := make([][]float64, numberRowsA)
	for i := range matrix {
		matrix[i] = make([]float64, numberRowsB)
	}
	for i := 0; i < numberRowsA; i++ {
		for j := 0; j < numberRowsB; j++ {
			for k := 0; k < numberColsA; k++ {
				matrix[i][j] += A[i][k] * B[k][j]
			}
		}
	}
	return matrix
}

func luSolve(A [][]float64, B []float64) ([]float64, []float64) {
	n := len(A)
	L := make([][]float64, n)
	U := make([][]float64, n)
	for i := range L {
		L[i] = make([]float64, n)
		U[i] = make([]float64, n)
	}
	for i := 0; i < n; i++ {
		L[i][i] = 1.0
		for j := i; j < n; j++ {
			s := 0.0
			for k := 0; k < i; k++ {
				s += L[i][k] * U[k][j]
			}
			U[i][j] = A[i][j] - s
		}
		for j := i + 1; j < n; j++ {
			s := 0.0
			for k := 0; k < i; k++ {
				s += L[j][k] * U[k][i]
			}
			L[j][i] = (A[j][i] - s) / U[i][i]
		}
	}
	Y := make([]float64, n)
	for i := 0; i < n; i++ {
		s := 0.0
		for j := 0; j < i; j++ {
			s += L[i][j] * Y[j]
		}
		Y[i] = B[i] - s
	}
	X := make([]float64, n)
	for i := n - 1; i >= 0; i-- {
		s := 0.0
		for j := i + 1; j < n; j++ {
			s += U[i][j] * X[j]
		}
		X[i] = (Y[i] - s) / U[i][i]
	}
	return Y, X
}

func seidelSolve(A [][]float64, B []float64, eps float64) []SeidelIteration {
	n := len(A)
	AT := make([][]float64, n)
	for i := range AT {
		AT[i] = make([]float64, n)
		for j := 0; j < n; j++ {
			AT[i][j] = A[j][i]
		}
	}
	Anew := multiply(AT, A)
	Bm := make([][]float64, n)
	for i := 0; i < n; i++ {
		Bm[i] = []float64{B[i]}
	}
	Bnewm := multiply(AT, Bm)
	Bnew := make([]float64, n)
	for i := 0; i < n; i++ {
		Bnew[i] = Bnewm[i][0]
	}
	X := make([]float64, n)
	diff := eps + 1.0
	iter := 0
	var history []SeidelIteration
	for diff > eps {
		diff = 0.0
		for i := 0; i < n; i++ {
			old := X[i]
			s := 0.0
			for j := 0; j < n; j++ {
				if i != j {
					s += Anew[i][j] * X[j]
				}
			}
			X[i] = (Bnew[i] - s) / Anew[i][i]
			if math.Abs(X[i]-old) > diff {
				diff = math.Abs(X[i] - old)
			}
		}
		iter++
		X_copy := make([]float64, n)
		copy(X_copy, X)
		history = append(history, SeidelIteration{Iter: iter, X: X_copy, Diff: diff})
	}
	return history
}

func printLuResults(Y, X []float64) {
	fmt.Println("Прямой ход метода LU")
	for i := 0; i < len(Y); i++ {
		fmt.Printf("y%d = %.5f\n", i+1, Y[i])
	}
	fmt.Println("\nОбратный ход метода LU")
	for i := 0; i < len(X); i++ {
		fmt.Printf("x%d = %.5f\n", i+1, X[i])
	}
}

func printSeidelResults(history []SeidelIteration) {
	fmt.Println("\nИтерационный метод Зейделя")
	fmt.Printf("%5s | %10s | %10s | %10s | %10s | %10s\n", "N", "X1", "X2", "X3", "X4", "En")
	fmt.Println(strings.Repeat("-", 65))
	for _, step := range history {
		fmt.Printf("%5d | %10.5f | %10.5f | %10.5f | %10.5f | %10.5f\n",
			step.Iter, step.X[0], step.X[1], step.X[2], step.X[3], step.Diff)
	}
}

func RunSlauSolver() {
	M := -1.19
	N_val := -0.21
	P := 1.21

	A := [][]float64{
		{M, -0.04, 0.21, -1.16},
		{0.25, -1.23, N_val, -0.09},
		{-0.21, N_val, 0.8, -0.13},
		{0.15, -1.31, 0.06, P},
	}
	B := []float64{-1.24, P, 2.56, M}
	eps := 1e-3

	Y, X := luSolve(A, B)
	printLuResults(Y, X)

	seidelHistory := seidelSolve(A, B, eps)
	printSeidelResults(seidelHistory)
}
