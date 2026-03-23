#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

class PotionRecipe {
public:
    std::string potionName;
    std::vector<std::string> ingredients;
    int quantity;

    PotionRecipe(const std::string& name, const std::vector<std::string>& ingredients)
        : potionName(name), ingredients(ingredients), quantity(3) {
    }
};

class AlchemyWorkshop {
private:
    std::vector<PotionRecipe> recipes;
    const int MAX_STOCK = 3;

public:

    void addRecipe(const std::string& name, const std::vector<std::string>& ingredients) {
        recipes.push_back(PotionRecipe(name, ingredients));
        std::cout << ">> [시스템] 새로운 레시피 '" << name << "'이(가) 추가되었습니다. (초기 재고: 3)" << std::endl;
    }

    std::vector<int> searchPotions(const std::string& query) const {
        std::vector<int> results;
        for (int i = 0; i < recipes.size(); ++i) {

            if (recipes[i].potionName == query) {
                results.push_back(i);
                continue;
            }

            for (const auto& ing : recipes[i].ingredients) {
                if (ing == query) {
                    results.push_back(i);
                    break;
                }
            }
        }
        return results;
    }

    void givePotion(int index) {
        if (recipes[index].quantity > 0) {
            recipes[index].quantity--;
            std::cout << ">> [지급 완료] '" << recipes[index].potionName << "'을 모험가에게 주었습니다." << std::endl;
            std::cout << "   (남은 재고: " << recipes[index].quantity << " / " << MAX_STOCK << ")" << std::endl;
        }
        else {
            std::cout << ">> [오류] 재고가 부족하여 지급할 수 없습니다!" << std::endl;
        }
    }

    void returnBottle(int index) {
        if (recipes[index].quantity < MAX_STOCK) {
            recipes[index].quantity++;
            std::cout << ">> [반환 완료] 공병을 받아 재고를 채웠습니다." << std::endl;
            std::cout << "   (현재 재고: " << recipes[index].quantity << " / " << MAX_STOCK << ")" << std::endl;
        }
        else {
            std::cout << ">> [경고] 이미 창고가 꽉 찼습니다! (최대 " << MAX_STOCK << "개)" << std::endl;
        }
    }

    void displayAllRecipes() const {
        if (recipes.empty()) {
            std::cout << "등록된 레시피가 없습니다." << std::endl;
            return;
        }
        std::cout << "\n--- [ 공방 창고 현황 ] ---" << std::endl;
        for (const auto& r : recipes) {
            std::cout << "- " << r.potionName << " [재고: " << r.quantity << "/" << MAX_STOCK << "]" << std::endl;
            std::cout << "  재료: ";
            for (size_t i = 0; i < r.ingredients.size(); ++i) {
                std::cout << r.ingredients[i] << (i == r.ingredients.size() - 1 ? "" : ", ");
            }
            std::cout << "\n";
        }
        std::cout << "--------------------------\n";
    }

    std::string getPotionName(int index) const { return recipes[index].potionName; }
};

int main() {
    AlchemyWorkshop workshop;
    int choice;

    while (true) {
        std::cout << "\n⚗️ 연금술 공방 관리 시스템" << std::endl;
        std::cout << "1. 레시피 추가" << std::endl;
        std::cout << "2. 모든 목록 및 재고 확인" << std::endl;
        std::cout << "3. 물약 찾기 및 지급/반환" << std::endl;
        std::cout << "4. 종료" << std::endl;
        std::cout << "선택: ";

        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            continue;
        }

        if (choice == 1) {
            std::string name, ing;
            std::vector<std::string> ings;
            std::cout << "물약 이름: ";
            std::cin.ignore();
            std::getline(std::cin, name);
            while (true) {
                std::cout << "재료 입력 ('끝' 입력 시 종료): ";
                std::getline(std::cin, ing);
                if (ing == "끝") break;
                ings.push_back(ing);
            }
            if (!ings.empty()) workshop.addRecipe(name, ings);

        }
        else if (choice == 2) {
            workshop.displayAllRecipes();

        }
        else if (choice == 3) {
            std::string query;
            std::cout << "검색 (이름 또는 재료): ";
            std::cin.ignore();
            std::getline(std::cin, query);

            std::vector<int> founds = workshop.searchPotions(query);
            if (founds.empty()) {
                std::cout << ">> 일치하는 결과가 없습니다." << std::endl;
            }
            else {
                std::cout << "\n[검색 결과]" << std::endl;
                for (int i = 0; i < founds.size(); ++i) {
                    std::cout << i + 1 << ". " << workshop.getPotionName(founds[i]) << std::endl;
                }
                std::cout << "상호작용할 번호 선택 (취소는 0): ";
                int targetIdx;
                std::cin >> targetIdx;

                if (targetIdx > 0 && targetIdx <= founds.size()) {
                    int finalIdx = founds[targetIdx - 1];
                    std::cout << "1. 지급(-1) | 2. 반환(+1) : ";
                    int action;
                    std::cin >> action;
                    if (action == 1) workshop.givePotion(finalIdx);
                    else if (action == 2) workshop.returnBottle(finalIdx);
                }
            }
        }
        else if (choice == 4) {
            break;
        }
    }
    return 0;
}