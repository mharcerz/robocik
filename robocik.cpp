#include <iostream>

#define _x first
#define _y second
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

enum starting_point {
    not_know = 1, after_two_moves = 2, after_four_moves = 4
};

using namespace std;

int d[100005]; // table of moves
int n, x, y, x_after_d, y_after_d;
// number of moves, searching point x, searching point y, position x after turn, position y after turn
pair<int, int> steps[100005]; // next robot's cartesian points in first turn

long long pref_turn_time[100005]; // the time it takes to get to the next Cartesian robot points
long long final_answer, t, time_for_d; // final answer, available time, time needed to complete the entire turn

void input();

int which_case();

long long repetitively(int); // repetitive movements, the robot returns to the starting point(0,0)

void parameters();

long long both_not_back(int, int, int, int, int, int, int, long long); //both parameters not go back to start(0,0)

long long one_goes_back(int, int, int, int, int, int, long long); // one of the parameters goes back to start

int one_not_goes_back(int, int, int, int, long long); // one of the parameters not goes back to start (second of one_goes_back)

int when_top_of_segment_pass_y(int, int, int);

int when_down_of_segment_pass_y(int, int, int);

int how_many_cross(long long, long long); // how many times the point passes

int main() {

    input();

    switch (which_case()) {
        case after_four_moves:
            final_answer = repetitively(after_four_moves);
            break;
        case after_two_moves:
            final_answer = repetitively(after_two_moves);
            break;
        case not_know: {
            parameters();
            if (x_after_d == 0 && y_after_d == 0) {
                final_answer = repetitively(not_know);
                break;
            } else { // the robot does not return to the starting point (0,0)
                long long potential_add_answer, answer = 0;

                if (x_after_d != 0 && y_after_d != 0) { //  both parameters do not come back
                    for (int i = 1; i <= n; i++) {

                        if (i % 4 == 1) // top
                            potential_add_answer = both_not_back(x, y, steps[i]._x, x_after_d, y_after_d,
                                                                 steps[i + 1]._y,steps[i]._y, pref_turn_time[i - 1]);
                        else if (i % 4 == 2) // right
                            potential_add_answer = both_not_back(y, x, steps[i]._y, y_after_d, x_after_d,
                                                                 steps[i + 1]._x,steps[i]._x, pref_turn_time[i - 1]);
                        else if (i % 4 == 3) // down
                            potential_add_answer = both_not_back(x, y * -1, steps[i]._x, x_after_d, y_after_d * -1,
                                                                 steps[i + 1]._y * -1, steps[i]._y * -1,pref_turn_time[i - 1]);
                        else if (i % 4 == 0) // left
                            potential_add_answer = both_not_back(y, x * -1, steps[i]._y, y_after_d, x_after_d * -1,
                                                                 steps[i + 1]._x * -1, steps[i]._x * -1,pref_turn_time[i - 1]);

                        if (potential_add_answer >= 0 && t >= potential_add_answer) // if the transition time is within the range
                            answer++;
                    }
                    final_answer = answer;
                    break;

                } else if (x_after_d == 0 && y_after_d != 0) { // one parameter comes back, one does not

                    for (int i = 1; i <= n; i++) {

                        if (i % 4 == 1 && steps[i]._x == x) { // top
                            potential_add_answer = one_not_goes_back(y, y_after_d, steps[i + 1]._y,
                                                                     steps[i]._y, pref_turn_time[i - 1]);
                            answer += potential_add_answer;
                        } else if (i % 4 == 2) { // right
                            potential_add_answer = one_goes_back(x, y, steps[i]._y, y_after_d, steps[i + 1]._x,
                                                                 steps[i]._x, pref_turn_time[i - 1]);

                            if (potential_add_answer > 0 && potential_add_answer <= t) // if the transition time is within the range
                                answer++;

                        } else if (i % 4 == 3 && steps[i]._x == x) { // down

                            potential_add_answer = one_not_goes_back(y * -1, y_after_d * -1, steps[i + 1]._y * -1,
                                                                     steps[i]._y * -1, pref_turn_time[i - 1]);
                            answer += potential_add_answer;
                        } else if (i % 4 == 0) { // left
                            potential_add_answer = one_goes_back(x * -1, y, steps[i]._y, y_after_d, steps[i + 1]._x * -1,
                                                                 steps[i]._x * -1, pref_turn_time[i - 1]);  ///zwraca czy dodać

                            if (potential_add_answer > 0 && potential_add_answer <= t) // if the transition time is within the range
                                answer++;
                        }
                    }
                    final_answer = answer;
                    break;

                } else if (x_after_d != 0 && y_after_d == 0) { // one parameter doesn't come back, one comes back

                    for (int i = 1; i <= n; i++) {

                        if (i % 4 == 1) { // top
                            potential_add_answer = one_goes_back(y, x, steps[i]._x, x_after_d, steps[i + 1]._y,
                                                                 steps[i]._y, pref_turn_time[i - 1]);

                            if (potential_add_answer > 0 && potential_add_answer <= t) // if the transition time is within the range
                                answer++;
                        } else if (i % 4 == 2 && steps[i]._y == y) { // right

                            potential_add_answer = one_not_goes_back(x, x_after_d, steps[i + 1]._x, steps[i]._x, pref_turn_time[i - 1]);
                            answer += potential_add_answer;
                        } else if (i % 4 == 3) { // down
                            potential_add_answer = one_goes_back(y * -1, x, steps[i]._x, x_after_d,steps[i + 1]._y * -1,
                                                                 steps[i]._y * -1, pref_turn_time[i - 1]);

                            if (potential_add_answer > 0 && potential_add_answer <= t) // if the transition time is within the range
                                answer++;

                        } else if (i % 4 == 0 && steps[i]._y == y) { // left

                            potential_add_answer = one_not_goes_back(x * -1, x_after_d * -1, steps[i + 1]._x * -1,
                                                                     steps[i]._x * -1, pref_turn_time[i - 1]);
                            answer += potential_add_answer;
                        }
                    }
                    final_answer = answer;
                    break;
                }
            }
        }
        default:
            break;
    }
    if (x == 0 && y == 0) // if the point is the starting point, we found it at the beginning
        final_answer++;

    cout << final_answer << endl;
    return EXIT_SUCCESS;
}

void input() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    cin >> n >> t;

    for (int i = 1; i <= n; i++) {
        cin >> d[i]; // moves
    }
    cin >> x >> y; // searching points
}

int which_case() {
    if (n % 2 == 1)
        return after_four_moves; // comes back after 4 turns
    else if (n % 2 == 0 && n % 4 != 0)
        return after_two_moves; // comes back after 2 turns
    else //if (n % 2 == 0 && n % 4 == 0)
        return not_know;
}

long long repetitively(int how_many_loops) {

    int j = 1, answer = 0, last_turns, move = 1;
    long long  fin_answer,  present_x = 0, present_y = 0, rest_time_turns_count, turns_count;
    long long time_to_return = 0; // the time it takes to return
    long long time_of_touching_point[100005];

    for (int ii = 1; ii <= how_many_loops; ii++) {
        for (int i = 1; i <= n; i++) { // robot movement simulation

            if (move % 4 == 1) { // top

                if (x == present_x && y > present_y && y <= present_y + d[i]) { // if we found point

                    time_of_touching_point[j++] = time_to_return + (y - present_y); // time when this happened
                    answer++; // add to answer
                }
                present_y += d[i]; // move point
            } else if (move % 4 == 2) { // right

                if (y == present_y && x > present_x && x <= present_x + d[i]) { // if we found point

                    time_of_touching_point[j++] = time_to_return + (x - present_x); // time when this happened
                    answer++; // add to answer
                }
                present_x += d[i]; // move point
            } else if (move % 4 == 3) { // down

                if (x == present_x && y < present_y && y >= present_y - d[i]) { // if we found point

                    time_of_touching_point[j++] = time_to_return + (present_y - y); // time when this happened
                    answer++; // add to answer
                }
                present_y -= d[i]; // move point
            } else if (move % 4 == 0) { // left

                if (y == present_y && x < present_x && x >= present_x - d[i]) { // if we found point

                    time_of_touching_point[j++] = time_to_return + (present_x - x); // time when this happened
                    answer++; // add to answer
                }
                present_x -= d[i]; // move point
            }
            move++; // move counter
            time_to_return += d[i] + 1;
        }
    }
    turns_count = t / time_to_return;
    rest_time_turns_count = t % time_to_return;
    last_turns = answer;

    for (int i = j - 1; i > 0; i--) {
        if (rest_time_turns_count >= time_of_touching_point[i])
            break;
        else // if time ran out
            last_turns--;
    }

    fin_answer = turns_count * answer + last_turns; // fin answer = number of turns to return * numbers of answer to return + numbers of answer in last turn
    return fin_answer;
}

void parameters() {

    int present_x = 0, present_y = 0;

    for (int i = 1; i <= n; i++) {
        if (i % 4 == 1) // top
            present_y += d[i];
        else if (i % 4 == 2) // right
            present_x += d[i];
        else if (i % 4 == 3) // down
            present_y -= d[i];
        else if (i % 4 == 0) // left
            present_x -= d[i];

        time_for_d += d[i] + 1;
        pref_turn_time[i] = time_for_d; // the time it takes to get to the next Cartesian robot points
        steps[i + 1]._x = present_x; // next robot's x point in first turn
        steps[i + 1]._y = present_y; // next robot's y point in first turn
    }
    x_after_d = present_x; // point x after turn
    y_after_d = present_y; // point y after turn
}
 // We will modify the call so that the robot always goes up or down
long long both_not_back(int searching_x, int searching_y, int present_x, int x_after_turn, int y_after_turn,
                        int y_end, int y_begin, long long time_to_get_y_begin) {

    if ((searching_x - present_x) % x_after_turn == 0) {
        // the point you are looking for is above or below you at the same width*how_many_turns x, so you can proceed
        int how_many_turns; // number of turns before time runs out
        long long time_to_get_here;
        how_many_turns = (searching_x - present_x) / x_after_turn;

        if (how_many_turns < 0) // we are going in the wrong direction
            return -1;

        y_begin += y_after_turn * how_many_turns; // start of segment x that will intersect the search value
        y_end += y_after_turn * how_many_turns; // start of segment y that will intersect the search value
        time_to_get_here = how_many_turns * time_for_d + time_to_get_y_begin + (searching_y - y_begin);
        // the time it takes to get to the point

            if (searching_y > y_begin && searching_y <= y_end) // if the y segment intersects the search value
                return time_to_get_here;
            else
                return -1; // we didn't find the point
    }
    return -1; // we didn't find the point
}
// We will modify the call so that the robot will always walk to the side
long long one_goes_back(int searching_x, int searching_y, int present_y, int y_after_turn,
                        int x_end, int x_begin, long long time_to_get_x_begin) {

    if ((searching_y - present_y) % y_after_turn == 0 && x_begin < searching_x && searching_x <= x_end) {
        // the point you are looking for is to the right or left at the same height*how_many_turns y,
        // and x is in the range, so you can proceed
        int how_many_turns;
        long long time_to_get_here;

        how_many_turns = (searching_y - present_y) / y_after_turn;

        if (how_many_turns < 0) // we are going in the wrong direction
            return -1;

        time_to_get_here = how_many_turns * time_for_d + time_to_get_x_begin + abs(searching_x - x_begin);

        if (searching_x == x_end) // if x is at the end of the segment, we'll consider that next time
            return -1;

        return time_to_get_here;
    } else
        return -1;
}
// We will modify the call so that the robot always goes up or down
int one_not_goes_back(int searching_y, int y_after_turn, int y_end, int y_begin, long long time_to_get_y_begin) {
    int first_pass, last_pass, difference, crosses;
    long long time_of_first_get_point;

    if (y_after_turn > 0) { // for positive numbers

        first_pass = when_top_of_segment_pass_y(y_end, y_after_turn, searching_y);
        last_pass = when_down_of_segment_pass_y(y_begin, y_after_turn, searching_y) - 1;
    } else { // for negative numbers

        first_pass = when_top_of_segment_pass_y(-y_begin, -y_after_turn, -searching_y);
        last_pass = when_down_of_segment_pass_y(-y_end, -y_after_turn, -searching_y) - 1;
    }

    if (first_pass > last_pass)
        // if the first crossing requires more moves than the last one, we're going in the wrong direction
        return 0;

    difference = last_pass - first_pass + 1; // the total number of intersections of the point does not have time
    y_begin += first_pass * y_after_turn; // the beginning of a new segment
    time_of_first_get_point = first_pass * time_for_d + time_to_get_y_begin + (searching_y - y_begin);

    crosses = how_many_cross(time_of_first_get_point, time_for_d - y_after_turn); // includes time
    return min(crosses, difference);
}

int when_top_of_segment_pass_y(int top_of_segment, int jump, int searching_y) {
    int how_many_jumps = (searching_y - top_of_segment) / jump;
    if ((searching_y - top_of_segment) % jump > 0) how_many_jumps++;
    return max(how_many_jumps, 0);
}

int when_down_of_segment_pass_y(int down_of_segment, int jump, int searching_y) {
    if (searching_y < down_of_segment)
        return -1;

    int how_many_jumps = (searching_y - down_of_segment) / jump;
    how_many_jumps++;

    return how_many_jumps;
}

int how_many_cross(long long time_of_first_get_point, long long time_to_get_this_point_next_turn) {

    if (time_of_first_get_point > t)
        return 0;

    int how_many_cross_point = (t - time_of_first_get_point) / time_to_get_this_point_next_turn + 1;

    return how_many_cross_point;
}






