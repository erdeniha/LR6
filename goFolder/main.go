package main

import (
	"fmt"
	"main/coinBoard"
	"main/slau"
)

func main() {
	for {
		fmt.Println("1. Система уравнений")
		fmt.Println("2. Доска с монетами")
		fmt.Println("3. Выход")

		var choice int
		if _, err := fmt.Scan(&choice); err != nil {
			break
		}

		switch choice {
		case 1:
			slau.RunSlauSolver()
		case 2:
			coinBoard.RunChessboardSolver()
		case 3:
			fmt.Println("Выход из программы.")
			return
		default:
			fmt.Println("Неверный пункт меню.")
		}
		fmt.Println()
	}
}
