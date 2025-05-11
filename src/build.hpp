#pragma once
#include <string>
#include <vector>
#include "raylib.h" // Jeśli korzystasz z raylib
#include <iostream>

enum TaskType {
    DELIVER_LIST,     // Dostarczenie listu
    GET_LIST,        // Odebranie listu
    TAKE_PHOTO,      // Zrobienie zdjęcia
    DELIVER_PHOTO,   // Dostarczenie zdjęcia
    GET_ZADANIE,     // Odbiór zadania (np. zrobienie zdjęcia)
};

// Struktura reprezentująca zadanie
struct Task {
    TaskType type;   // Typ zadania
    bool accepted;

    Task(TaskType t);
};

// Struktura reprezentująca budynek
struct Building {
    std::string name;
    Vector2 position;
    Texture2D texture;
    mutable Task task;  // Lista zadań dla budynku

    Texture2D envelope;
    mutable bool envpVisible = false;
    mutable bool enpGet = true;
    mutable Vector2 envpPosition = {0, 0};
    mutable float envpSpeed = 2.0f;

    RenderTexture2D photoRender;
    mutable Texture2D photoTexture;
    mutable bool photoReady = false;
    mutable bool photoTaken = false;
    mutable bool photoDelivered = false;
    mutable float photoTimer = 0.0f;
    mutable bool photoFlashActive = false;
    mutable float photoFlashAlpha = 0.0f;

    // Dodajemy zmienne do animacji
    mutable float scale = 1.0f; // Początkowy rozmiar (1.0 oznacza pełny rozmiar)
    mutable bool isAnimating = false; // Flaga, która mówi, czy budynek jest w trakcie animacji
    mutable bool isActiated = false;
    float scaleSpeed = 0.01f; // Szybkość zmiany rozmiaru w animacji

    Building(const std::string& buildingName, const Vector2& buildingPosition, const Texture2D& buildingTexture, const Task& buildingTask);

    void AddTask(TaskType type);
    void AcceptTask();
    void ShowTasks() const;
    void doTask() const;  // Nowa metoda do wykonania zadania
    void Draw() const;
    void Animate(int& countTake, int& countLeave) const;
    Rectangle GetRect() const;
};

// Funkcja do inicjalizacji wszystkich budynków
void InitBuildings(std::vector<Building>& buildings);

