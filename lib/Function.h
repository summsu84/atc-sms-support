/*
 * Function.h
 *
 *  Created on: 2012. 10. 31.
 *      Author: jiwon
 */

#ifndef FUNCTION_H_
#define FUNCTION_H_

enum Shape{Line, Points, Polyline, Polygon, Rect, RoundedRect, Ellipse, Arc, Chord, Pie, Path, Text, Pixmap};

enum{
	LAYER_LEVEL_0 = 0,
	LAYER_LEVEL_1 = 100,
	LAYER_LEVEL_2 = 110,
	LAYER_LEVEL_3 = 120
};
enum LineType{
	SOLID  = 1,
	DASHED = 2,
	DOTTED = 3

};

enum Mode{
	INSERT_MODE,
	REPLACE_MODE,
	DB_MODE,
	FILE_MODE
};
enum FillPattern{
	SOLID_F 				= 1,
	DOTTED_F				= 2,
	HORIZONTALLINE 		= 3,
	VERTICALLINE 		= 4,
	CROSSLINE 			= 5,
	SLASHED 				= 6,
	REVERSESLASHED 		= 7,
	CROSSSLASHED 		= 9
};

#endif /* FUNCTION_H_ */
