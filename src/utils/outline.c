#include "../../include/hashTableDefine.h"

//Find the cosest empty cell to draw on
Point findClosestEmpty(Point cell, Grid grid) {
	for (int i = 0; i < 15; i++) {
		if (grid.grid[cell.y + i][cell.x] == 0 && grid.tour_grid[cell.y + i][cell.x] == 0)
			return (Point) {cell.x, cell.y + i};
		if (grid.grid[cell.y][cell.x + i] == 0 && grid.tour_grid[cell.y][cell.x + i] == 0)
			return (Point) {cell.x + i, cell.y};
		if (grid.grid[cell.y - i][cell.x] == 0 && grid.tour_grid[cell.y - i][cell.x] == 0)
			return (Point) {cell.x, cell.y - i};
		if (grid.grid[cell.y][cell.x - i] == 0 && grid.tour_grid[cell.y][cell.x - i] == 0)
			return (Point) {cell.x - i, cell.y};
	}
	return (Point) {cell.x, cell.y};
}


//Check if a cell has neighbhors (filled cells on grid.grid)
Neighbor contactPoint(Point c, GameData *game_data) {
	Neighbor new = {
		.dir = {0, 0, 0, 0},
		.valid = false,
	};
	if (c.y + 1 < 15 && (game_data->grid.grid[c.y + 1][c.x]  != 0 || game_data->grid.tour_grid[c.y + 1][c.x])) {
		if (game_data->grid.grid[c.y + 1][c.x] != 0)
			new.valid = true;
		new.dir[SOUTH] = 1;
	}
	if (c.x + 1 < 15 && (game_data->grid.grid[c.y][c.x + 1]  != 0 || game_data->grid.tour_grid[c.y][c.x + 1])) {
		if (game_data->grid.grid[c.y][c.x + 1] != 0)
			new.valid = true;
		new.dir[EAST] = 1;
	}
	if (c.y - 1 > 0 && (game_data->grid.grid[c.y - 1][c.x]  != 0 || game_data->grid.tour_grid[c.y - 1][c.x])) {
		if (game_data->grid.grid[c.y - 1][c.x] != 0)
			new.valid = true;
		new.dir[NORTH] = 1;
	}
	if (c.x - 1 > 0 && (game_data->grid.grid[c.y][c.x - 1]  != 0 || game_data->grid.tour_grid[c.y][c.x - 1])) {
		if (game_data->grid.grid[c.y][c.x - 1] != 0)
			new.valid = true;
		new.dir[WEST] = 1;
	}
	return new;
}

//return number of ruler cells that are on the grid
int countRulerOnGrid(Ruler ruler) {
	int x = 0;

	for (int i = 0; i < 7; i++) {
		if (ruler.cell[i].x != -1)
			x++;
	}
	return x;
}

//Check if all ruler's cells are aligned on an axis.
int checkAlignment(GameData *game_data) {
	int x_ref = -1, y_ref = -1;
	int l_x = 16, l_y = 16;
	int h_x = -1, h_y = -1;
	bool ref = true;

	//If there are no rulers on the grid return false
	if (countRulerOnGrid(game_data->ruler) == 1)
		return 0;

	else {

		for (int i = 0; i < 7; i++) {
			//Skip ruler evaluation if its not on the grid
			if (game_data->ruler.cell[i].x == -1)
				continue;
			//Set coordinate reference to the first ruler on the grid.
			if (ref) {
				x_ref = game_data->ruler.cell[i].x;
				y_ref = game_data->ruler.cell[i].y;
				ref = false;
				continue;
			}

			//Update coordinate reference to match the farthest right and down
			if (game_data->ruler.cell[i].x > h_x) h_x = game_data->ruler.cell[i].x;
			if (game_data->ruler.cell[i].y > h_y) h_y = game_data->ruler.cell[i].y;
			if (game_data->ruler.cell[i].x < l_x) l_x = game_data->ruler.cell[i].x;
			if (game_data->ruler.cell[i].y < l_y) l_y = game_data->ruler.cell[i].y;

			//Reset ruler's ref if a ruler is not the same axis
			if (game_data->ruler.cell[i].x != x_ref)
				x_ref = -1;
			if (game_data->ruler.cell[i].y != y_ref)
				y_ref = -1;
		}

		//If rulers aren't aligned return false
		if (x_ref == -1 && y_ref == -1)
			return -1;

		//If they're aligned on x axis but there's en empty cell in between them return false
		if (x_ref != -1) {
			for (; l_y <= h_y; l_y++) {
				if (game_data->grid.grid[l_y][x_ref] == 0 && game_data->grid.tour_grid[l_y][x_ref] == 0)
					return -1;
			}
		}

		//If they're aligned on y axis but there's en empty cell in between them return false
		if (y_ref != -1) {
			for (; l_x <= h_x; l_x++) {
				if (game_data->grid.grid[y_ref][l_x] == 0 && game_data->grid.tour_grid[y_ref][l_x] == 0)
					return -1;
			}
		}
	}
	return 0;
}

