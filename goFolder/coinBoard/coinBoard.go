package coinBoard

import "fmt"

func printMask(mask, N, M int) {
	for i := 0; i < N; i++ {
		for j := 0; j < M; j++ {
			bitPos := i*M + j
			bitValue := (mask >> bitPos) & 1
			fmt.Printf("%d ", bitValue)
		}
		fmt.Println()
	}
	fmt.Println()
}

func getTargetMasks(N, M int) (int, int) {
	target1 := 0
	target2 := 0
	for i := 0; i < N; i++ {
		for j := 0; j < M; j++ {
			bitPos := i*M + j
			if (i+j)%2 == 0 {
				target1 |= (1 << bitPos)
			} else {
				target2 |= (1 << bitPos)
			}
		}
	}
	return target1, target2
}

func solveChessboard(N, M, startMask int) []int {
	target1, target2 := getTargetMasks(N, M)
	if startMask == target1 || startMask == target2 {
		return []int{startMask}
	}
	var q []int
	dist := make(map[int]int)
	parent := make(map[int]int)
	q = append(q, startMask)
	dist[startMask] = 0
	parent[startMask] = -1
	endMask := -1
	for len(q) > 0 {
		curr := q[0]
		q = q[1:]
		d := dist[curr]
		if curr == target1 || curr == target2 {
			endMask = curr
			break
		}
		for i := 0; i < N; i++ {
			for j := 0; j < M; j++ {
				pos1 := i*M + j
				if j+1 < M {
					pos2 := i*M + (j + 1)
					nextMask := curr ^ (1 << pos1) ^ (1 << pos2)
					if _, exists := dist[nextMask]; !exists {
						dist[nextMask] = d + 1
						parent[nextMask] = curr
						q = append(q, nextMask)
					}
				}
				if i+1 < N {
					pos2 := (i+1)*M + j
					nextMask := curr ^ (1 << pos1) ^ (1 << pos2)
					if _, exists := dist[nextMask]; !exists {
						dist[nextMask] = d + 1
						parent[nextMask] = curr
						q = append(q, nextMask)
					}
				}
			}
		}
	}
	if endMask == -1 {
		return nil
	}
	var path []int
	currNode := endMask
	for currNode != -1 {
		path = append(path, currNode)
		currNode = parent[currNode]
	}
	for i, j := 0, len(path)-1; i < j; i, j = i+1, j-1 {
		path[i], path[j] = path[j], path[i]
	}
	return path
}

func RunChessboardSolver() {
	var N, M int
	fmt.Print("Введите размеры доски N и M: ")
	if _, err := fmt.Scan(&N, &M); err != nil {
		return
	}
	fmt.Println("Введите матрицу состояний (0 - орел, 1 - решка):")
	startMask := 0
	for i := 0; i < N; i++ {
		for j := 0; j < M; j++ {
			var val int
			fmt.Scan(&val)
			if val == 1 {
				startMask |= (1 << (i*M + j))
			}
		}
	}
	path := solveChessboard(N, M, startMask)
	if path == nil {
		fmt.Println("\nРешения не существует")
		return
	}
	if len(path) == 1 {
		fmt.Println("\nНачальное состояние уже является целевым!")
		printMask(path[0], N, M)
		fmt.Println("Минимальное число ходов: 0")
		return
	}
	fmt.Println("\nПромежуточные состояния доски")
	for i := 0; i < len(path); i++ {
		if i == 0 {
			fmt.Println("Начальное состояние:")
		} else {
			fmt.Printf("Ход №%d:\n", i)
		}
		printMask(path[i], N, M)
	}
	fmt.Printf("Минимальное число ходов: %d\n", len(path)-1)
}
