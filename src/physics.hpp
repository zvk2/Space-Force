
int FOR_LIMIT = 3;
int BACK_LIMIT = -3;
int NEUTRAL_X_FOR = 0;
int NEUTRAL_X_BACK = 0;
int NEUTRAL_Y_UP = 0;
int NEUTRAL_Y_DOWN = 0;
//the max speed the player can go
void Speed_Limit(int* x_vel, int *y_vel)
{
		// Check speed limits

		if (*x_vel < BACK_LIMIT)

			*x_vel = BACK_LIMIT;

		else if (*x_vel > FOR_LIMIT)

			*x_vel = FOR_LIMIT;



		if (*y_vel < BACK_LIMIT)

			*y_vel = BACK_LIMIT;

		else if (*y_vel > FOR_LIMIT)

			*y_vel = FOR_LIMIT;
			return;
}
//What to do when no buttons are pressed
void Neutral(int *x_del, int *y_del, int *x_vel, int *y_vel)
{
	if(*x_del == 0)
	{
		if(*x_vel > NEUTRAL_X_FOR)
			{
				*x_del = -1;
			}
			else if (*x_vel < NEUTRAL_X_BACK)
			{
				*x_del = 1;
			}
	}
	if (*y_del == 0)
	{
		if (*y_vel > NEUTRAL_Y_UP)
		{
			*y_del = -1;
		}
		else if (*y_vel < NEUTRAL_Y_DOWN)
		{
			*y_del = 1;
		}
	}
	return;
}

