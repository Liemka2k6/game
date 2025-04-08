#ifndef _DEFS_H
#define _DEFS_H

// Kích thước cửa sổ trò chơi
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;
const int starty1=670;
const int starty2=45;
const int endButtonX = SCREEN_WIDTH - 70;
const int endButtonY = SCREEN_HEIGHT - 70;
const int endButtonRadius = 30;
// Kích thước lá bài
const int CARD_WIDTH = 70;
const int CARD_HEIGHT = 102;
// Số lượng người chơi
const int NUM_PLAYERS = 2;

// Tổng số thẻ bài trong trò chơi
const int TOTAL_CARDS = 40;

// Định nghĩa vị trí hiển thị bài trên màn hình
const int PLAYER_HAND_Y = SCREEN_HEIGHT - CARD_HEIGHT - 20; // Vị trí của bài người chơi
const int OPPONENT_HAND_Y = 60; // Vị trí bài đối thủ

// Thời gian delay giữa các lượt (miligiây)
const int TURN_DELAY = 1000;

#endif // _DEFS_H
