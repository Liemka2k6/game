#ifndef _GAMEPLAY_H
#define _GAMEPLAY_H

#include "graphics.h"
#include "Card.h"
#include "defs.h"
#include "Deck.h"
#include "zones.h"
#include "rendering.h"
#include <vector>
#include <iostream>
class Gameplay
{
public:
    Deck playerdeck;
    Deck opponentdeck;                           // Bộ bài chính
    std::vector<Card> playerHand,playerHand1;        // Bài trên tay người chơi
    std::vector<Card> opponentHand,opponentHand1;      // Bài trên tay đối thủ
    std::vector<Card> monsterplayerField;
    std::vector<Card> monsteropponentField;
    std::vector<Card> playerGraveyard;
    std::vector<Card> opponentGraveyard;
    // Khởi tạo game: phát mỗi người 3 lá
    Gameplay()
        : monsterplayerField(5), monsteropponentField(5)
    {
        startGame();
    }

    // Bắt đầu ván mới
    void startGame()
    {
        playerdeck.shuffleDeck();
        SDL_Delay(1);
        opponentdeck.shuffleDeck();
        playerHand.clear();
        opponentHand.clear();
    }

    // Mỗi lượt: người chơi bốc thêm 1 lá nếu còn trong bộ bài
    void playerDrawCard()
    {
        if (!playerdeck.cards.empty())
        {
            playerHand.push_back(playerdeck.cards.back());
            playerdeck.cards.pop_back();
        }
    }
    void renderSingleCardOnHand(Graphics& graphics, zones &zones, Card& card, int startX, int startY)
    {
        // Tải texture của lá bài
        SDL_Texture* tex = graphics.loadTexture(card.imagePath.c_str());


        // Vị trí để vẽ lá bài
        SDL_Rect dst = { startX, startY, CARD_WIDTH, CARD_HEIGHT };
        zones.handZonesP1.push_back(dst);
        // Vẽ lá bài lên renderer
        SDL_RenderCopy(graphics.renderer, tex, nullptr, &dst);
        // Giải phóng texture sau khi vẽ xong
        SDL_DestroyTexture(tex);
    }

    void opponentrenderSingleCardOnHand(Graphics& graphics,zones &zones, Card& card, int startX, int startY)
    {
        // Tải texture của lá bài
        SDL_Texture* tex = graphics.loadTexture("images//Mặt_sau_lá_bài.jpg");

        // Vị trí để vẽ lá bài
        SDL_Rect dst = { startX, startY, CARD_WIDTH, CARD_HEIGHT };
        zones.handZonesP2.push_back(dst);
        // Vẽ lá bài lên renderer
        SDL_RenderCopy(graphics.renderer, tex, nullptr, &dst);
        // Giải phóng texture sau khi vẽ xong
        //   SDL_DestroyTexture(tex);
    }

    // Đối thủ cũng bốc 1 lá
    void opponentDrawCard()
    {
        if (!opponentdeck.cards.empty())
        {
            opponentHand.push_back(opponentdeck.cards.back());
            opponentdeck.cards.pop_back();
        }
    }
    int clickhand(int mouseX, int mouseY, zones& zones)
    {
        SDL_Point point = { mouseX, mouseY };
        for (size_t i = 0; i < zones.handZonesP1.size(); ++i)
        {
            if (SDL_PointInRect(&point, &zones.handZonesP1[i]))
            {
                return static_cast<int>(i); // Trả về chỉ số vùng được click
            }
        }
        return -1; // Không click vào vùng nào
    }
    int clickfield(int mouseX, int mouseY, zones& zones)
    {
        SDL_Point point = { mouseX, mouseY };
        for (size_t i = 0; i < zones.monsterZonesP1.size(); ++i)
        {
            if (SDL_PointInRect(&point, &zones.monsterZonesP1[i]))
            {
                return static_cast<int>(i); // Trả về chỉ số vùng được click
            }
        }
        return -1; // Không click vào vùng nào
    }
    void handleSummonClick(int mouseX, int mouseY,int i, int i1, zones& zones, Graphics& graphics)
    {
        // Kiểm tra ô trống (ví dụ kiểm tra xem trong vector monster đang có lá nào hay không)
        // Nếu bạn chưa có vector chứa các lá bài ở monsterZoneP1 thì tạo 1 vector nhé

        // Render lá bài tại vị trí monsterZoneP1[summonIndex]
        // Xoá khỏi tay
        removeCardWithAnimation(i, i1, zones, graphics);
        // Reset trạng thái
    }
    void removeCardWithAnimation(size_t indexToMove, int targetZoneIndex, zones& zones, Graphics& graphics)
    {
        const int spacing = 71;
        const int animationFrames = 20;

        if (indexToMove >= playerHand.size()) return;

        // Lưu lá bài cần chuyển động
        Card cardToMove = playerHand[indexToMove];
        std::vector<Card> originalHand = playerHand;

        // Xoá khỏi tay trước khi vẽ animation
        playerHand.erase(playerHand.begin() + indexToMove);

        for (int frame = 0; frame <= animationFrames; ++frame)
        {
            double t = static_cast<double>(frame) / animationFrames;
            graphics.prepareScene(graphics.loadTexture("images//background.jpg"));

            for (size_t i = 0; i < originalHand.size(); ++i)
            {
                SDL_Texture* tex = graphics.loadTexture(originalHand[i].imagePath.c_str());

                if (i == indexToMove)
                {
                    // Animate lá bài chuyển động
                    int startX = zones.handZonesP1[indexToMove].x;
                    int startY = zones.handZonesP1[indexToMove].y;
                    int endX = zones.monsterZonesP1[targetZoneIndex].x;
                    int endY = zones.monsterZonesP1[targetZoneIndex].y;

                    int currentX = static_cast<int>(startX + (endX - startX) * t);
                    int currentY = static_cast<int>(startY + (endY - startY) * t);

                    SDL_Rect movingRect = {currentX, currentY, CARD_WIDTH, CARD_HEIGHT};
                    SDL_RenderCopy(graphics.renderer, tex, nullptr, &movingRect);
                }
                else
                {
                    int newX = zones.handZonesP1[i].x;
                    if (i > indexToMove) newX -= static_cast<int>(spacing * t);
                    SDL_Rect drawRect = {newX, zones.handZonesP1[i].y, CARD_WIDTH, CARD_HEIGHT};
                    SDL_RenderCopy(graphics.renderer, tex, nullptr, &drawRect);
                }

                SDL_DestroyTexture(tex);
            }

            renderZoneOutlines(graphics, zones);
            renderOpponentHand(graphics, opponentHand, zones);
            renderMonstersOnField(graphics, monsterplayerField, zones.monsterZonesP1);
            renderOpponentMonsters(graphics, monsteropponentField, zones.monsterZonesP2);
            renderDecks(graphics, zones);
            renderGraveyards(graphics, playerGraveyard, opponentGraveyard, zones.playerGraveyardZone, zones.opponentGraveyardZone);

            graphics.presentScene();
        }

        // Gán bài lên sân sau khi animation hoàn tất
        monsterplayerField[targetZoneIndex] = cardToMove;

        // Cập nhật lại zone bài trên tay
        std::vector<SDL_Rect> targetRects;
        for (size_t i = 0; i < playerHand.size(); ++i)
        {
            SDL_Rect dst = {300 + static_cast<int>(i) * spacing, starty1, CARD_WIDTH, CARD_HEIGHT};
            targetRects.push_back(dst);
        }

        zones.handZonesP1 = targetRects;
    }

    void opponentremoveCardWithAnimation(zones& zones, Graphics& graphics)
    {
        const int spacing = 71;
        const int animationFrames = 20;

        int maxATK = -1e6, chooseHand = -1, chooseField = -1;

        // Tìm bài ATK cao nhất trong tay
        for (int i = 0; i < static_cast<int>(opponentHand.size()); ++i)
        {
            if (opponentHand[i].atk > maxATK)
            {
                maxATK = opponentHand[i].atk;
                chooseHand = i;
            }
        }

        // Tìm slot trống đầu tiên trên sân
        for (int i = 0; i < 5; ++i)
        {
            if (monsteropponentField[i].name.empty())
            {
                chooseField = i;
                break;
            }
        }

        if (chooseHand == -1 || chooseField == -1) return;

        Card cardToMove = opponentHand[chooseHand];
        std::vector<Card> originalHand = opponentHand;

        opponentHand.erase(opponentHand.begin() + chooseHand);

        for (int frame = 0; frame <= animationFrames; ++frame)
        {
            double t = static_cast<double>(frame) / animationFrames;
            graphics.prepareScene(graphics.loadTexture("images//background.jpg"));

            for (size_t i = 0; i < originalHand.size(); ++i)
            {
                if (i == chooseHand) continue;

                SDL_Texture* tex = graphics.loadTexture("images//Mặt_sau_lá_bài.jpg");
                int newX = zones.handZonesP2[i].x;
                if (i > chooseHand) newX -= static_cast<int>(spacing * t);

                SDL_Rect drawRect = {newX, zones.handZonesP2[i].y, CARD_WIDTH, CARD_HEIGHT};
                SDL_RenderCopy(graphics.renderer, tex, nullptr, &drawRect);
                SDL_DestroyTexture(tex);
            }

            // Animate bài chuyển lên sân
            int startX = zones.handZonesP2[chooseHand].x;
            int startY = zones.handZonesP2[chooseHand].y;
            int endX = zones.monsterZonesP2[chooseField].x;
            int endY = zones.monsterZonesP2[chooseField].y;

            int currentX = static_cast<int>(startX + (endX - startX) * t);
            int currentY = static_cast<int>(startY + (endY - startY) * t);

            SDL_Rect movingRect = {currentX, currentY, CARD_WIDTH, CARD_HEIGHT};
            SDL_Texture* tex = graphics.loadTexture(cardToMove.imagePath.c_str());
            SDL_RenderCopy(graphics.renderer, tex, nullptr, &movingRect);
            SDL_DestroyTexture(tex);

            renderZoneOutlines(graphics, zones);
            renderPlayerHand(graphics, playerHand, zones);
            renderMonstersOnField(graphics, monsterplayerField, zones.monsterZonesP1);
            renderOpponentMonsters(graphics, monsteropponentField, zones.monsterZonesP2);
            renderDecks(graphics, zones);
            renderGraveyards(graphics, playerGraveyard, opponentGraveyard, zones.playerGraveyardZone, zones.opponentGraveyardZone);

            graphics.presentScene();
        }

        // Gán bài lên sân
        monsteropponentField[chooseField] = cardToMove;

        // Cập nhật lại zone tay
        std::vector<SDL_Rect> newRects;
        for (size_t i = 0; i < opponentHand.size(); ++i)
        {
            SDL_Rect rect = {300 + static_cast<int>(i) * spacing, starty2, CARD_WIDTH, CARD_HEIGHT};
            newRects.push_back(rect);
        }

        zones.handZonesP2 = newRects;
    }

    int selectedAttackerIndex = -1;
int selectedTargetIndex = -1;

void handleAttackClick(int mouseX, int mouseY, zones& zones, Graphics& graphics, Mix_Chunk* attackSound)
{
    SDL_Point point = {mouseX, mouseY};

    // Bước 1: chọn quái tấn công
    if (selectedAttackerIndex == -1)
    {
        for (size_t i = 0; i < zones.monsterZonesP1.size(); ++i)
        {
            if (SDL_PointInRect(&point, &zones.monsterZonesP1[i]) && monsterplayerField[i].name != "")
            {
                selectedAttackerIndex = static_cast<int>(i);
                return;
            }
        }
    }
    // Bước 2: chọn mục tiêu hoặc tấn công trực tiếp
    else if (selectedTargetIndex == -1)
    {
        bool opponentHasMonster = false;
        for (const auto& card : monsteropponentField)
        {
            if (card.name != "")
            {
                opponentHasMonster = true;
                break;
            }
        }

        if (!opponentHasMonster)
        {
            // 🗡 Tấn công trực tiếp
            if (attackSound) Mix_PlayChannel(-1, attackSound, 0);
            // TODO: giảm máu đối thủ ở đây nếu có biến máu

            // 🌄 Cập nhật lại màn hình với background
            SDL_Texture* background = graphics.loadTexture("images//background.jpg");
            graphics.prepareScene(background);
            renderZoneOutlines(graphics, zones);
            renderPlayerHand(graphics, playerHand, zones);
            renderOpponentHand(graphics, opponentHand, zones);
            renderMonstersOnField(graphics, monsterplayerField, zones.monsterZonesP1);
            renderOpponentMonsters(graphics, monsteropponentField, zones.monsterZonesP2);
            renderGraveyards(graphics, playerGraveyard, opponentGraveyard,
                             zones.playerGraveyardZone, zones.opponentGraveyardZone);
            renderDecks(graphics, zones);
            graphics.presentScene();

            selectedAttackerIndex = -1;
            return;
        }

        // Có quái → kiểm tra vùng chọn mục tiêu
        for (size_t i = 0; i < zones.monsterZonesP2.size(); ++i)
        {
            if (SDL_PointInRect(&point, &zones.monsterZonesP2[i]) && monsteropponentField[i].name != "")
            {
                selectedTargetIndex = static_cast<int>(i);

                // 🔁 Animation tấn công
                animateAttackOnlyAttackerMoves(
                    selectedAttackerIndex, selectedTargetIndex,
                    true,  // isPlayerAttacking
                    monsterplayerField, monsteropponentField,
                    zones, graphics
                );

                // ⚔ Tính toán kết quả tấn công
                int attackerATK = monsterplayerField[selectedAttackerIndex].atk;
                int targetATK = monsteropponentField[selectedTargetIndex].atk;

                if (attackerATK > targetATK)
                {
                    opponentGraveyard.push_back(monsteropponentField[selectedTargetIndex]);
                    monsteropponentField[selectedTargetIndex] = Card();
                }
                else if (attackerATK < targetATK)
                {
                    playerGraveyard.push_back(monsterplayerField[selectedAttackerIndex]);
                    monsterplayerField[selectedAttackerIndex] = Card();
                }
                else
                {
                    opponentGraveyard.push_back(monsteropponentField[selectedTargetIndex]);
                    playerGraveyard.push_back(monsterplayerField[selectedAttackerIndex]);
                    monsteropponentField[selectedTargetIndex] = Card();
                    monsterplayerField[selectedAttackerIndex] = Card();
                }

                if (attackSound) Mix_PlayChannel(-1, attackSound, 0);

                // 🌄 Vẽ lại toàn bộ với background
                SDL_Texture* background = graphics.loadTexture("images//background.jpg");
                graphics.prepareScene(background);
                renderZoneOutlines(graphics, zones);
                renderPlayerHand(graphics, playerHand, zones);
                renderOpponentHand(graphics, opponentHand, zones);
                renderMonstersOnField(graphics, monsterplayerField, zones.monsterZonesP1);
                renderOpponentMonsters(graphics, monsteropponentField, zones.monsterZonesP2);
                renderGraveyards(graphics, playerGraveyard, opponentGraveyard,
                                 zones.playerGraveyardZone, zones.opponentGraveyardZone);
                renderDecks(graphics, zones);
                graphics.presentScene();

                // Reset lựa chọn
                selectedAttackerIndex = -1;
                selectedTargetIndex = -1;
                return;
            }
        }
    }
}

    void animateAttackOnlyAttackerMoves(int attackerIndex, int defenderIndex, bool isPlayerAttacking,
                                        std::vector<Card>& attackerField, std::vector<Card>& defenderField,
                                        zones& zones, Graphics& graphics)
    {
        const int steps = 30;
        SDL_Rect startRect = isPlayerAttacking ? zones.monsterZonesP1[attackerIndex] : zones.monsterZonesP2[attackerIndex];
        SDL_Rect endRect   = isPlayerAttacking ? zones.monsterZonesP2[defenderIndex] : zones.monsterZonesP1[defenderIndex];

        Card attackerCard = attackerField[attackerIndex];
        SDL_Texture* attackerTexture = graphics.loadTexture(attackerCard.imagePath.c_str());

        for (int i = 0; i <= steps; ++i)
        {
            graphics.prepareScene();

            // --- Vẽ sân và tất cả các thành phần ---
            renderZoneOutlines(graphics, zones);
            renderDecks(graphics, zones);
            renderGraveyards(graphics, playerGraveyard, opponentGraveyard,
                             zones.playerGraveyardZone, zones.opponentGraveyardZone);
            renderPlayerHand(graphics, playerHand, zones);
            renderOpponentHand(graphics, opponentHand, zones);
            renderMonstersOnField(graphics, monsterplayerField, zones.monsterZonesP1);
            renderOpponentMonsters(graphics, monsteropponentField, zones.monsterZonesP2);

            // Tính toán vị trí animation
            float t = static_cast<float>(i) / steps;
            int currentX = static_cast<int>(startRect.x + t * (endRect.x - startRect.x));
            int currentY = static_cast<int>(startRect.y + t * (endRect.y - startRect.y));
            SDL_Rect attackerRect = { currentX, currentY, CARD_WIDTH, CARD_HEIGHT };

            SDL_RenderCopy(graphics.renderer, attackerTexture, nullptr, &attackerRect);
            graphics.presentScene();
            SDL_Delay(10);
        }

        SDL_DestroyTexture(attackerTexture);
    }



};

#endif // _GAMEPLAY_H


