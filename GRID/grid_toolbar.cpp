#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <fstream>
#include <sstream>

using namespace std;

// === CONSTANTS ===
const float VIRTUAL_WIDTH = 1600.0f;
const float VIRTUAL_HEIGHT = 1200.0f;
const float TOOLBAR_WIDTH = 250.0f;
const string CONFIG_FILE = "grid_config.txt";

struct Config {
    int rows, cols;
    bool showTop, showBottom, showLeft, showRight;
    bool colLeftToRight; 
    bool rowTopToBottom; 
    int baseIndex;
    
    // Default constructor
    Config() : rows(10), cols(10), showTop(true), showBottom(false), 
               showLeft(true), showRight(false), colLeftToRight(true), 
               rowTopToBottom(true), baseIndex(0) {}
};

// UI Button Structure
struct Button {
    sf::FloatRect rect;
    sf::Color color;
    string label;
    int actionID; // -1: Clear, -2: Eraser, -3: Size+, -4: Size-, -5: Settings, -6: Undo, -7: Redo, 0-9: Colors
};

struct InputField {
    sf::FloatRect rect;
    string value;
    bool active;
    int maxLength;
    bool numbersOnly;
    
    InputField() : active(false), maxLength(10), numbersOnly(false) {}
};

struct Checkbox {
    sf::FloatRect rect;
    string label;
    bool* valuePtr;
};

// Save config to file
void saveConfig(const Config& cfg) {
    ofstream file(CONFIG_FILE);
    if (file.is_open()) {
        file << cfg.rows << " " << cfg.cols << "\n";
        file << cfg.showTop << " " << cfg.showBottom << " " << cfg.showLeft << " " << cfg.showRight << "\n";
        file << cfg.colLeftToRight << " " << cfg.rowTopToBottom << "\n";
        file << cfg.baseIndex << "\n";
        file.close();
    }
}

// Load config from file
Config loadConfig() {
    Config cfg;
    ifstream file(CONFIG_FILE);
    if (file.is_open()) {
        file >> cfg.rows >> cfg.cols;
        file >> cfg.showTop >> cfg.showBottom >> cfg.showLeft >> cfg.showRight;
        file >> cfg.colLeftToRight >> cfg.rowTopToBottom;
        file >> cfg.baseIndex;
        file.close();
        
        // Validate loaded values
        if (cfg.rows <= 0 || cfg.rows > 100) cfg.rows = 10;
        if (cfg.cols <= 0 || cfg.cols > 100) cfg.cols = 10;
        if (cfg.baseIndex < 0 || cfg.baseIndex > 1) cfg.baseIndex = 0;
    }
    return cfg;
}

bool loadFont(sf::Font &font) {
    vector<string> paths = {
        "/usr/share/fonts/truetype/ubuntu/Ubuntu-R.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "/usr/share/fonts/truetype/freefont/FreeSans.ttf",
        "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf",
        "arial.ttf"
    };
    for (const auto &path : paths) {
        if (font.loadFromFile(path)) return true;
    }
    return false;
}

void recalculateLayout(const Config& cfg, float& padTop, float& padBot, float& padLeft, 
                       float& padRight, float& gridW, float& gridH, float& cellW, float& cellH) {
    float drawAreaWidth = VIRTUAL_WIDTH - TOOLBAR_WIDTH;
    
    padTop = cfg.showTop ? 60.0f : 20.0f;
    padBot = cfg.showBottom ? 60.0f : 20.0f;
    padLeft = cfg.showLeft ? 60.0f : 20.0f;
    padRight = cfg.showRight ? 60.0f : 20.0f;

    gridW = drawAreaWidth - padLeft - padRight;
    gridH = VIRTUAL_HEIGHT - padTop - padBot;
    cellW = gridW / cfg.cols;
    cellH = gridH / cfg.rows;
}

int main() {
    // --- LOAD CONFIG ---
    Config cfg = loadConfig();
    
    cout << "=== GRID PAINTER PRO ===" << endl;
    cout << "Loaded settings: " << cfg.rows << "x" << cfg.cols << " grid" << endl;
    cout << "Click 'Settings' button in the toolbar to configure!" << endl;

    // --- WINDOW SETUP ---
    sf::RenderWindow window(sf::VideoMode(1000, 750), "Grid Painter Pro");
    window.setFramerateLimit(120);

    sf::View view(sf::FloatRect(0, 0, VIRTUAL_WIDTH, VIRTUAL_HEIGHT));
    window.setView(view);

    // --- DRAWING SURFACE ---
    sf::RenderTexture canvas;
    if (!canvas.create(VIRTUAL_WIDTH, VIRTUAL_HEIGHT)) return -1;
    canvas.clear(sf::Color(30, 30, 30));

    // --- FONT ---
    sf::Font font;
    if (!loadFont(font)) cerr << "Warning: No font found." << endl;

    // --- LAYOUT CALCULATIONS ---
    float padTop, padBot, padLeft, padRight, gridW, gridH, cellW, cellH;
    recalculateLayout(cfg, padTop, padBot, padLeft, padRight, gridW, gridH, cellW, cellH);

    // --- PALETTE & TOOLS ---
    vector<sf::Color> palette = {
        sf::Color(30, 30, 30),   // 0: Eraser (Match BG)
        sf::Color::White,        // 1
        sf::Color(231, 76, 60),  // 2: Red
        sf::Color(46, 204, 113), // 3: Green
        sf::Color(52, 152, 219), // 4: Blue
        sf::Color(241, 196, 15), // 5: Yellow
        sf::Color(155, 89, 182), // 6: Purple
        sf::Color(230, 126, 34), // 7: Orange
        sf::Color(26, 188, 156), // 8: Teal
        sf::Color(149, 165, 166) // 9: Grey
    };

    int currentColorIdx = 1;
    float brushSize = 5.0f;
    bool eraserActive = false;
    bool showSettings = false;

    // --- BUILD UI BUTTONS ---
    vector<Button> buttons;
    float uiX = VIRTUAL_WIDTH - TOOLBAR_WIDTH + 25.0f;
    float uiY = 50.0f;

    // 1. Color Palette Grid
    for (size_t i = 1; i < palette.size(); i++) {
        Button b;
        b.rect = sf::FloatRect(uiX + ((i-1)%2) * 105, uiY + ((i-1)/2) * 105, 95, 95);
        b.color = palette[i];
        b.actionID = i;
        b.label = "";
        buttons.push_back(b);
    }
    uiY += 550.0f;

    // 2. Eraser
    buttons.push_back({sf::FloatRect(uiX, uiY, 200, 60), sf::Color(50, 50, 50), "Eraser", -2});
    uiY += 80.0f;

    // 3. Size Controls
    buttons.push_back({sf::FloatRect(uiX, uiY, 95, 60), sf::Color(70, 70, 70), "Size -", -4});
    buttons.push_back({sf::FloatRect(uiX + 105, uiY, 95, 60), sf::Color(70, 70, 70), "Size +", -3});
    uiY += 80.0f;

    // 4. Clear Board
    buttons.push_back({sf::FloatRect(uiX, uiY, 200, 60), sf::Color(192, 57, 43), "CLEAR", -1});
    uiY += 80.0f;

    // 5. Undo/Redo
    buttons.push_back({sf::FloatRect(uiX, uiY, 95, 60), sf::Color(100, 100, 100), "Undo", -6});
    buttons.push_back({sf::FloatRect(uiX + 105, uiY, 95, 60), sf::Color(100, 100, 100), "Redo", -7});
    uiY += 80.0f;

    // 6. Settings Button
    buttons.push_back({sf::FloatRect(uiX, uiY, 200, 60), sf::Color(41, 128, 185), "Settings", -5});

    // --- SETTINGS PANEL SETUP ---
    float settingsPanelX = 100.0f;
    float settingsPanelY = 100.0f;
    float settingsPanelW = 500.0f;
    float settingsPanelH = 700.0f;

    // Input fields
    vector<InputField> inputFields(3);
    inputFields[0].rect = sf::FloatRect(settingsPanelX + 200, settingsPanelY + 60, 100, 40);
    inputFields[0].value = to_string(cfg.rows);
    inputFields[0].numbersOnly = true;
    inputFields[0].maxLength = 3;
    
    inputFields[1].rect = sf::FloatRect(settingsPanelX + 200, settingsPanelY + 120, 100, 40);
    inputFields[1].value = to_string(cfg.cols);
    inputFields[1].numbersOnly = true;
    inputFields[1].maxLength = 3;
    
    inputFields[2].rect = sf::FloatRect(settingsPanelX + 200, settingsPanelY + 180, 100, 40);
    inputFields[2].value = to_string(cfg.baseIndex);
    inputFields[2].numbersOnly = true;
    inputFields[2].maxLength = 1;

    // Checkboxes
    vector<Checkbox> checkboxes;
    float cbY = settingsPanelY + 250;
    checkboxes.push_back({sf::FloatRect(settingsPanelX + 30, cbY, 25, 25), "Show Top Labels", &cfg.showTop});
    cbY += 50;
    checkboxes.push_back({sf::FloatRect(settingsPanelX + 30, cbY, 25, 25), "Show Bottom Labels", &cfg.showBottom});
    cbY += 50;
    checkboxes.push_back({sf::FloatRect(settingsPanelX + 30, cbY, 25, 25), "Show Left Labels", &cfg.showLeft});
    cbY += 50;
    checkboxes.push_back({sf::FloatRect(settingsPanelX + 30, cbY, 25, 25), "Show Right Labels", &cfg.showRight});
    cbY += 50;
    checkboxes.push_back({sf::FloatRect(settingsPanelX + 30, cbY, 25, 25), "Columns: Left to Right", &cfg.colLeftToRight});
    cbY += 50;
    checkboxes.push_back({sf::FloatRect(settingsPanelX + 30, cbY, 25, 25), "Rows: Top to Bottom", &cfg.rowTopToBottom});

    // Apply and Close buttons
    sf::FloatRect applyButtonRect(settingsPanelX + 50, settingsPanelY + settingsPanelH - 80, 180, 50);
    sf::FloatRect closeButtonRect(settingsPanelX + 270, settingsPanelY + settingsPanelH - 80, 180, 50);

    // --- STATE VARIABLES ---
    bool isDrawing = false;
    sf::Vector2f lastPos;

    // --- UNDO/REDO SYSTEM ---
    const int MAX_HISTORY = 50;
    vector<sf::Image> history;
    int historyIndex = -1;

    // Function to save current canvas state
    auto saveState = [&]() {
        // Remove any redo states if we're not at the end
        if (historyIndex < (int)history.size() - 1) {
            history.erase(history.begin() + historyIndex + 1, history.end());
        }
        
        // Save current state
        sf::Image currentState = canvas.getTexture().copyToImage();
        history.push_back(currentState);
        
        // Limit history size
        if (history.size() > MAX_HISTORY) {
            history.erase(history.begin());
        } else {
            historyIndex++;
        }
    };

    // Function to restore a state
    auto restoreState = [&](int index) {
        if (index >= 0 && index < (int)history.size()) {
            sf::Texture temp;
            temp.loadFromImage(history[index]);
            sf::Sprite sprite(temp);
            canvas.clear(sf::Color(30, 30, 30));
            canvas.draw(sprite);
            canvas.display();
            historyIndex = index;
        }
    };

    // Save initial empty state
    saveState();

    // --- MAIN LOOP ---
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            
            // Handle Resize
            if (event.type == sf::Event::Resized) {
                float aspectRatio = float(window.getSize().x) / float(window.getSize().y);
                view.setSize(VIRTUAL_HEIGHT * aspectRatio, VIRTUAL_HEIGHT);
                view.setCenter(VIRTUAL_WIDTH / 2, VIRTUAL_HEIGHT / 2);
                window.setView(view);
            }

            // Keyboard shortcuts
            if (event.type == sf::Event::KeyPressed) {
                bool ctrlPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || 
                                  sf::Keyboard::isKeyPressed(sf::Keyboard::RControl);
                
                if (ctrlPressed && event.key.code == sf::Keyboard::Z) {
                    // Undo
                    if (historyIndex > 0) {
                        restoreState(historyIndex - 1);
                    }
                } else if (ctrlPressed && event.key.code == sf::Keyboard::Y) {
                    // Redo
                    if (historyIndex < (int)history.size() - 1) {
                        restoreState(historyIndex + 1);
                    }
                }
            }

            // Text Input
            if (event.type == sf::Event::TextEntered && showSettings) {
                for (auto& field : inputFields) {
                    if (field.active) {
                        if (event.text.unicode == 8) { // Backspace
                            if (!field.value.empty()) field.value.pop_back();
                        } else if (event.text.unicode == 13) { // Enter
                            field.active = false;
                        } else if (event.text.unicode < 128) {
                            char c = static_cast<char>(event.text.unicode);
                            if (field.numbersOnly && !isdigit(c)) continue;
                            if (field.value.length() < field.maxLength) {
                                field.value += c;
                            }
                        }
                    }
                }
            }

            // Click Handling
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                
                // Settings Panel Active
                if (showSettings) {
                    // Check input fields
                    for (auto& field : inputFields) {
                        field.active = field.rect.contains(mPos);
                    }
                    
                    // Check checkboxes
                    for (auto& cb : checkboxes) {
                        if (cb.rect.contains(mPos)) {
                            *(cb.valuePtr) = !*(cb.valuePtr);
                        }
                    }
                    
                    // Apply button
                    if (applyButtonRect.contains(mPos)) {
                        // Parse and validate input
                        try {
                            int newRows = stoi(inputFields[0].value);
                            int newCols = stoi(inputFields[1].value);
                            int newBase = stoi(inputFields[2].value);
                            
                            if (newRows > 0 && newRows <= 100 && newCols > 0 && newCols <= 100 
                                && (newBase == 0 || newBase == 1)) {
                                cfg.rows = newRows;
                                cfg.cols = newCols;
                                cfg.baseIndex = newBase;
                                
                                // Recalculate layout
                                recalculateLayout(cfg, padTop, padBot, padLeft, padRight, gridW, gridH, cellW, cellH);
                                
                                // Save config
                                saveConfig(cfg);
                                
                                // Save canvas state after settings change
                                canvas.display();
                                saveState();
                                
                                showSettings = false;
                                cout << "Settings applied: " << cfg.rows << "x" << cfg.cols << " grid" << endl;
                            }
                        } catch (...) {
                            // Invalid input, ignore
                        }
                    }
                    
                    // Close button
                    if (closeButtonRect.contains(mPos)) {
                        // Restore values from config
                        inputFields[0].value = to_string(cfg.rows);
                        inputFields[1].value = to_string(cfg.cols);
                        inputFields[2].value = to_string(cfg.baseIndex);
                        showSettings = false;
                    }
                    
                } else {
                    // Check if click is in Toolbar
                    if (mPos.x > VIRTUAL_WIDTH - TOOLBAR_WIDTH) {
                        for (auto &b : buttons) {
                            if (b.rect.contains(mPos)) {
                                if (b.actionID > 0) { currentColorIdx = b.actionID; eraserActive = false; }
                                else if (b.actionID == -1) {
                                    canvas.clear(sf::Color(30, 30, 30));
                                    canvas.display();
                                    saveState();
                                }
                                else if (b.actionID == -2) eraserActive = true;
                                else if (b.actionID == -3) brushSize = min(50.0f, brushSize + 2.0f);
                                else if (b.actionID == -4) brushSize = max(2.0f, brushSize - 2.0f);
                                else if (b.actionID == -5) showSettings = true;
                                else if (b.actionID == -6) { // Undo
                                    if (historyIndex > 0) {
                                        restoreState(historyIndex - 1);
                                    }
                                }
                                else if (b.actionID == -7) { // Redo
                                    if (historyIndex < (int)history.size() - 1) {
                                        restoreState(historyIndex + 1);
                                    }
                                }
                            }
                        }
                    } else {
                        // Click is on Canvas
                        isDrawing = true;
                        lastPos = mPos;
                    }
                }
            }
            if (event.type == sf::Event::MouseButtonReleased) {
                if (isDrawing) {
                    saveState(); // Save state after drawing
                }
                isDrawing = false;
            }
        }

        // --- DRAWING LOGIC ---
        if (isDrawing && window.hasFocus() && !showSettings) {
            sf::Vector2f currPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            
            if (currPos.x < VIRTUAL_WIDTH - TOOLBAR_WIDTH) {
                sf::Color paintColor = eraserActive ? palette[0] : palette[currentColorIdx];

                float dist = std::hypot(currPos.x - lastPos.x, currPos.y - lastPos.y);
                int steps = static_cast<int>(dist / (brushSize / 4));
                if (steps < 1) steps = 1;

                for (int i = 0; i <= steps; i++) {
                    float t = (float)i / steps;
                    sf::Vector2f p = lastPos + (currPos - lastPos) * t;
                    sf::CircleShape brush(brushSize);
                    brush.setOrigin(brushSize, brushSize);
                    brush.setPosition(p);
                    brush.setFillColor(paintColor);
                    canvas.draw(brush);
                }
                canvas.display();
                lastPos = currPos;
            }
        }

        window.clear(sf::Color(20, 20, 20));

        // 1. Draw Canvas
        sf::Sprite sprite(canvas.getTexture());
        window.draw(sprite);

        // 2. Draw Grid
        sf::RectangleShape cell(sf::Vector2f(cellW, cellH));
        cell.setFillColor(sf::Color::Transparent);
        cell.setOutlineColor(sf::Color(80, 80, 80));
        cell.setOutlineThickness(2.0f);

        for (int r = 0; r < cfg.rows; r++) {
            for (int c = 0; c < cfg.cols; c++) {
                cell.setPosition(padLeft + c * cellW, padTop + r * cellH);
                window.draw(cell);
            }
        }

        // 3. Draw Grid Labels
        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::White);

        float drawAreaWidth = VIRTUAL_WIDTH - TOOLBAR_WIDTH;

        // Rows
        for(int r=0; r<cfg.rows; r++) {
            int val = cfg.rowTopToBottom ? (cfg.baseIndex + r) : (cfg.baseIndex + cfg.rows - 1 - r);
            text.setString(to_string(val));
            float yCenter = padTop + r * cellH + cellH/2.0f - 15.0f;
            if(cfg.showLeft) { text.setPosition(10, yCenter); window.draw(text); }
            if(cfg.showRight) { text.setPosition(drawAreaWidth - 50, yCenter); window.draw(text); }
        }
        // Cols
        for(int c=0; c<cfg.cols; c++) {
            int val = cfg.colLeftToRight ? (cfg.baseIndex + c) : (cfg.baseIndex + cfg.cols - 1 - c);
            text.setString(to_string(val));
            float xCenter = padLeft + c * cellW + cellW/2.0f - 10.0f;
            if(cfg.showTop) { text.setPosition(xCenter, 10); window.draw(text); }
            if(cfg.showBottom) { text.setPosition(xCenter, VIRTUAL_HEIGHT - 40); window.draw(text); }
        }

        // 4. Draw Toolbar Background
        sf::RectangleShape toolbarBg(sf::Vector2f(TOOLBAR_WIDTH, VIRTUAL_HEIGHT));
        toolbarBg.setPosition(VIRTUAL_WIDTH - TOOLBAR_WIDTH, 0);
        toolbarBg.setFillColor(sf::Color(45, 45, 45));
        toolbarBg.setOutlineColor(sf::Color(100, 100, 100));
        toolbarBg.setOutlineThickness(-2);
        window.draw(toolbarBg);

        // 5. Draw Buttons
        for (auto &b : buttons) {
            sf::RectangleShape shape(sf::Vector2f(b.rect.width, b.rect.height));
            shape.setPosition(b.rect.left, b.rect.top);
            
            // Check if button should be disabled (undo/redo)
            bool isDisabled = false;
            if (b.actionID == -6 && historyIndex <= 0) isDisabled = true; // Undo disabled
            if (b.actionID == -7 && historyIndex >= (int)history.size() - 1) isDisabled = true; // Redo disabled
            
            // Set color based on disabled state
            if (isDisabled) {
                shape.setFillColor(sf::Color(40, 40, 40));
            } else {
                shape.setFillColor(b.color);
            }
            
            bool isSelected = false;
            if (!eraserActive && b.actionID == currentColorIdx) isSelected = true;
            if (eraserActive && b.actionID == -2) isSelected = true;
            
            if (isSelected) {
                shape.setOutlineColor(sf::Color::White);
                shape.setOutlineThickness(4);
            } else {
                shape.setOutlineColor(sf::Color::Black);
                shape.setOutlineThickness(2);
            }
            window.draw(shape);

            if (!b.label.empty()) {
                sf::Text btnText;
                btnText.setString(b.label);
                btnText.setFont(font);
                btnText.setCharacterSize(20);
                btnText.setFillColor(isDisabled ? sf::Color(80, 80, 80) : sf::Color::White);
                sf::FloatRect textRect = btnText.getLocalBounds();
                btnText.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
                btnText.setPosition(b.rect.left + b.rect.width/2.0f, b.rect.top + b.rect.height/2.0f);
                window.draw(btnText);
            }
        }

        // 6. Draw Brush Preview
        sf::Text sizeLabel;
        sizeLabel.setString("Brush Size:");
        sizeLabel.setFont(font);
        sizeLabel.setCharacterSize(24);
        sizeLabel.setFillColor(sf::Color::White);
        sizeLabel.setPosition(VIRTUAL_WIDTH - TOOLBAR_WIDTH + 25, 960);
        window.draw(sizeLabel);

        sf::CircleShape preview(brushSize);
        preview.setOrigin(brushSize, brushSize);
        preview.setPosition(VIRTUAL_WIDTH - TOOLBAR_WIDTH + 125, 1060);
        preview.setFillColor(eraserActive ? palette[0] : palette[currentColorIdx]);
        window.draw(preview);

        // 7. Draw Settings Panel (if active)
        if (showSettings) {
            // Semi-transparent overlay
            sf::RectangleShape overlay(sf::Vector2f(VIRTUAL_WIDTH, VIRTUAL_HEIGHT));
            overlay.setFillColor(sf::Color(0, 0, 0, 180));
            window.draw(overlay);

            // Settings panel background
            sf::RectangleShape panel(sf::Vector2f(settingsPanelW, settingsPanelH));
            panel.setPosition(settingsPanelX, settingsPanelY);
            panel.setFillColor(sf::Color(50, 50, 50));
            panel.setOutlineColor(sf::Color(100, 100, 100));
            panel.setOutlineThickness(3);
            window.draw(panel);

            // Title
            sf::Text title;
            title.setString("Settings");
            title.setFont(font);
            title.setCharacterSize(32);
            title.setFillColor(sf::Color::White);
            title.setPosition(settingsPanelX + 20, settingsPanelY + 10);
            window.draw(title);

            // Input field labels and fields
            vector<string> labels = {"Rows (N):", "Columns (M):", "Base Index (0/1):"};
            for (size_t i = 0; i < inputFields.size(); i++) {
                sf::Text label;
                label.setString(labels[i]);
                label.setFont(font);
                label.setCharacterSize(20);
                label.setFillColor(sf::Color::White);
                label.setPosition(settingsPanelX + 30, inputFields[i].rect.top + 8);
                window.draw(label);

                sf::RectangleShape fieldBox(sf::Vector2f(inputFields[i].rect.width, inputFields[i].rect.height));
                fieldBox.setPosition(inputFields[i].rect.left, inputFields[i].rect.top);
                fieldBox.setFillColor(inputFields[i].active ? sf::Color(80, 80, 80) : sf::Color(60, 60, 60));
                fieldBox.setOutlineColor(inputFields[i].active ? sf::Color::White : sf::Color(100, 100, 100));
                fieldBox.setOutlineThickness(2);
                window.draw(fieldBox);

                sf::Text fieldText;
                fieldText.setString(inputFields[i].value + (inputFields[i].active ? "_" : ""));
                fieldText.setFont(font);
                fieldText.setCharacterSize(20);
                fieldText.setFillColor(sf::Color::White);
                fieldText.setPosition(inputFields[i].rect.left + 10, inputFields[i].rect.top + 8);
                window.draw(fieldText);
            }

            // Checkboxes
            for (auto& cb : checkboxes) {
                sf::RectangleShape box(sf::Vector2f(cb.rect.width, cb.rect.height));
                box.setPosition(cb.rect.left, cb.rect.top);
                box.setFillColor(sf::Color(60, 60, 60));
                box.setOutlineColor(sf::Color(100, 100, 100));
                box.setOutlineThickness(2);
                window.draw(box);

                if (*(cb.valuePtr)) {
                    sf::RectangleShape check(sf::Vector2f(15, 15));
                    check.setPosition(cb.rect.left + 5, cb.rect.top + 5);
                    check.setFillColor(sf::Color(46, 204, 113));
                    window.draw(check);
                }

                sf::Text cbLabel;
                cbLabel.setString(cb.label);
                cbLabel.setFont(font);
                cbLabel.setCharacterSize(20);
                cbLabel.setFillColor(sf::Color::White);
                cbLabel.setPosition(cb.rect.left + 35, cb.rect.top + 2);
                window.draw(cbLabel);
            }

            // Apply button
            sf::RectangleShape applyBtn(sf::Vector2f(applyButtonRect.width, applyButtonRect.height));
            applyBtn.setPosition(applyButtonRect.left, applyButtonRect.top);
            applyBtn.setFillColor(sf::Color(46, 204, 113));
            applyBtn.setOutlineColor(sf::Color::White);
            applyBtn.setOutlineThickness(2);
            window.draw(applyBtn);

            sf::Text applyText;
            applyText.setString("Apply");
            applyText.setFont(font);
            applyText.setCharacterSize(24);
            applyText.setFillColor(sf::Color::White);
            sf::FloatRect applyTextRect = applyText.getLocalBounds();
            applyText.setOrigin(applyTextRect.left + applyTextRect.width/2.0f, 
                               applyTextRect.top + applyTextRect.height/2.0f);
            applyText.setPosition(applyButtonRect.left + applyButtonRect.width/2.0f, 
                                 applyButtonRect.top + applyButtonRect.height/2.0f);
            window.draw(applyText);

            // Close button
            sf::RectangleShape closeBtn(sf::Vector2f(closeButtonRect.width, closeButtonRect.height));
            closeBtn.setPosition(closeButtonRect.left, closeButtonRect.top);
            closeBtn.setFillColor(sf::Color(192, 57, 43));
            closeBtn.setOutlineColor(sf::Color::White);
            closeBtn.setOutlineThickness(2);
            window.draw(closeBtn);

            sf::Text closeText;
            closeText.setString("Cancel");
            closeText.setFont(font);
            closeText.setCharacterSize(24);
            closeText.setFillColor(sf::Color::White);
            sf::FloatRect closeTextRect = closeText.getLocalBounds();
            closeText.setOrigin(closeTextRect.left + closeTextRect.width/2.0f, 
                               closeTextRect.top + closeTextRect.height/2.0f);
            closeText.setPosition(closeButtonRect.left + closeButtonRect.width/2.0f, 
                                 closeButtonRect.top + closeButtonRect.height/2.0f);
            window.draw(closeText);
        }

        window.display();
    }

    return 0;
}
