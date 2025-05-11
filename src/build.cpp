#include "build.hpp"

Task::Task(TaskType t) : type(t), accepted(false) {}

Building::Building(const std::string& buildingName, const Vector2& buildingPosition, const Texture2D& buildingTexture, const Task& buildingTask)
    : name(buildingName), position(buildingPosition), texture(buildingTexture), task(buildingTask) {
        envelope = LoadTexture("Graphics/1/envelope.png");
    }

void Building::AddTask(TaskType type) {
    task = Task(type);
}

void Building::AcceptTask() {
    task.accepted = true;
}

// void Building::ShowTasks() const {
//     for (const auto& task : tasks) {
//         std::cout << (task.accepted ? "[ACCEPTED] " : "[PENDING] ") << std::endl;
//     }
// }

void Building::doTask() const {
    switch (task.type) {
        case DELIVER_LIST:{
            if (!task.accepted) {
                std::cout << "List został dostarczony!" << std::endl;
                task.accepted = true;
                enpGet = false;
            }
            break;}
        case GET_LIST:{
            if (!task.accepted) {
                std::cout << "List został odebrany!" << std::endl;
                task.accepted = true;
                enpGet = true;
            }
            break;}
        case TAKE_PHOTO:{
            std::cout << "Zdjęcie zostało zrobione!" << std::endl;
            photoFlashActive = true;
            photoFlashAlpha = 1.0f;

            //TODO: poprawić wyświetlenie zrobionego zdjęcia
            if (!photoTaken) {
                // Wczytaj zdjęcie jako wynik "zrobienia zdjęcia"
                photoTexture = LoadTexture("Graphics/1/photo_taken.png"); // <- Twój plik JPG/PNG
                photoTaken = true;
                task.accepted = true;
            }
            break;
        }
        case GET_ZADANIE:{
            std::cout << "Zadanie zostało odebrane!" << std::endl;
            break;}
        case DELIVER_PHOTO:{
            if (photoTaken && !photoDelivered) {
                std::cout << "Zdjęcie zostało dostarczone!" << std::endl;
                photoDelivered = true;
                task.accepted = true;

                // Jeśli chcesz usunąć zdjęcie:
                UnloadRenderTexture(photoRender); // tylko raz, jeśli nie będziesz już go używać
            } else {
                std::cout << "Nie masz zdjęcia do dostarczenia!" << std::endl;
            }
            break;
        }
        default:
            std::cout << "Nieznane zadanie." << std::endl;
            break;
    }
}

void Building::Draw() const {
    float drawScale = scale * 0.25f;

    // Oblicz przesunięcie względem środka tekstury
    float offsetX = (texture.width * drawScale - texture.width * 0.25f) / 2;
    float offsetY = (texture.height * drawScale - texture.height * 0.25f) / 2;

    Vector2 drawPosition = {
        position.x - offsetX,
        position.y - offsetY
    };

    if (envpVisible) {
        DrawTextureEx(envelope, envpPosition, 0.0f, 0.5f, WHITE);  // zamiast gwiazdy na razie kółko
    }

    DrawTextureEx(texture, drawPosition, 0.0f, scale * 0.25f, WHITE); // Możesz użyć dowolnego koloru
    // Flash - błysk ekranu
    if (photoFlashActive) {
        DrawRectangle(0, 0, 1400, 1000, Color{255, 255, 255, (unsigned char)(photoFlashAlpha * 255)});
        photoFlashAlpha -= 0.05f;
        if (photoFlashAlpha <= 0.0f) {
            photoFlashAlpha = 0.0f;
            photoFlashActive = false;
        }
    }
    if (photoTaken) DrawTextureEx(photoTexture, {100, 100}, 0.0f, 0.5f, WHITE);  // dowolna pozycja

}

void Building::Animate(int& count, int& countLeave) const {
    if (task.accepted && !isActiated) {
        // Zmniejszanie, a potem zwiększanie budynku
        if (!isAnimating) {
            scale -= scaleSpeed; // Zmniejszamy budynek
            if (scale <= 0.5f) {
                scale = 0.5f; // Minimalny rozmiar
                // Kiedy osiągniemy najmniejszy rozmiar, rozpoczynamy powiększanie
                isAnimating = true;
                envpVisible = true;
                if (enpGet) envpPosition = { position.x + texture.width * 0.125f / 2, position.y + texture.height * 0.125f / 2 };
                    else envpPosition = { position.x + texture.width * 0.125f / 2, position.y - 22.0f}; //(texture.height * 0.125f - 20.0f) / 2 };
            }
        } else {
            scale += scaleSpeed; // Zwiększanie budynku
            if (enpGet) {envpPosition.y -= envpSpeed;}
                else envpPosition.y += envpSpeed;
            if (scale >= 1.0f) {
                scale = 1.0f; // Ostateczny rozmiar
                //isAnimating = true;
                envpVisible = false;
                isActiated = true;
                if (enpGet) count++;
                    else countLeave--;
            }
        }
    }
}

Rectangle Building::GetRect() const {
    return Rectangle{position.x, position.y, float(texture.width / 4.0f), float(texture.height / 4.0f)};
}

void InitBuildings(std::vector<Building>& buildings) {
    std::vector<std::string> buildingsName = {
        "kamienica.png",  "school.png",  "krawiec.png",  "trainSt.png",  "church.png",
        "kamienica2.png",  "kamienica3.png",  "pharma.png", "shop.png", "bice.png"
    };

    std::vector<Vector2> positions = {
        {120, 321}, {347, 180}, {658, 113}, {990, 93}, {610, 367},
        {863, 407}, {148, 609}, {939, 626}, {394, 497}, {636, 630}
    };

    // Wektor zadań, który będzie przypisany do każdego budynku
    std::vector<Task> taskOrder = {
        Task(DELIVER_LIST), Task(DELIVER_PHOTO), Task(DELIVER_LIST), Task(TAKE_PHOTO), Task(GET_LIST),//Task(GET_ZADANIE),
        Task(DELIVER_PHOTO), Task(GET_LIST), Task(TAKE_PHOTO), Task(DELIVER_LIST), Task(GET_LIST)
    };

    // Załóżmy, że `texture` to obrazek, który wczytujesz z dysku lub wczytujesz za pomocą raylib
    for (size_t i = 0; i < buildingsName.size(); ++i) {
        std::string path = "Graphics/1/" + buildingsName[i];
        Texture2D texture = LoadTexture(path.c_str());

        Building building(buildingsName[i], positions[i], texture, taskOrder[i]);

        buildings.push_back(building);
    }
}
