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

// === DATA STRUCTURES ===
struct Config {
    int rows, cols;
    bool showTop, showBottom, showLeft, showRight;
    bool colLeftToRight; 
    bool rowTopToBottom; 
    int baseIndex;
    
    Config() : rows(10), cols(10), showTop(true), showBottom(false), 
               showLeft(true), showRight(false), colLeftToRight(true), 
               rowTopToBottom(true), baseIndex(0) {}
};

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
    
    // Default constructor
    InputField() : active(false), maxLength(10), numbersOnly(false) {}

    // === ADD THIS CONSTRUCTOR ===
    InputField(sf::FloatRect r, string v, bool a, int m, bool n) 
        : rect(r), value(v), active(a), maxLength(m), numbersOnly(n) {}
};

struct Checkbox {
    sf::FloatRect rect;
    string label;
    bool* valuePtr;
};

// === HELPER FUNCTIONS ===

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

Config loadConfig() {
    Config cfg;
    ifstream file(CONFIG_FILE);
    if (file.is_open()) {
        file >> cfg.rows >> cfg.cols;
        file >> cfg.showTop >> cfg.showBottom >> cfg.showLeft >> cfg.showRight;
        file >> cfg.colLeftToRight >> cfg.rowTopToBottom;
        file >> cfg.baseIndex;
        file.close();
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
        "arial.ttf"
    };
    for (const auto &path : paths) {
        if (font.loadFromFile(path)) return true;
    }
    return false;
}

// --- OPTIMIZATION: Vertex Array Update ---
// Generates grid lines once, stores them in GPU memory
void updateGridGeometry(sf::VertexArray& gridLines, const Config& cfg, 
                        float padLeft, float padTop, float gridW, float gridH, float cellW, float cellH) {
    gridLines.clear();
    gridLines.setPrimitiveType(sf::Lines);
    sf::Color gridColor(80, 80, 80);

    // Vertical lines
    for (int c = 0; c <= cfg.cols; c++) {
        float x = floor(padLeft + c * cellW); // floor to snap to pixel
        float bottomY = padTop + cfg.rows * cellH;
        gridLines.append(sf::Vertex(sf::Vector2f(x, padTop), gridColor));
        gridLines.append(sf::Vertex(sf::Vector2f(x, bottomY), gridColor));
    }

    // Horizontal lines
    for (int r = 0; r <= cfg.rows; r++) {
        float y = floor(padTop + r * cellH);
        float rightX = padLeft + cfg.cols * cellW;
        gridLines.append(sf::Vertex(sf::Vector2f(padLeft, y), gridColor));
        gridLines.append(sf::Vertex(sf::Vector2f(rightX, y), gridColor));
    }
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

// === MAIN ===
int main() {
    // 1. Setup
    Config cfg = loadConfig();
    sf::RenderWindow window(sf::VideoMode(1000, 750), "Grid Painter Pro (Optimized)");
    // NOTE: FramerateLimit REMOVED. We use waitEvent for 0% idle CPU usage.
    
    sf::View view(sf::FloatRect(0, 0, VIRTUAL_WIDTH, VIRTUAL_HEIGHT));
    window.setView(view);

    sf::RenderTexture canvas;
    if (!canvas.create(VIRTUAL_WIDTH, VIRTUAL_HEIGHT)) return -1;
    canvas.clear(sf::Color(30, 30, 30));

    sf::Font font;
    if (!loadFont(font)) { cerr << "No font found." << endl; return -1; }

    // 2. Geometry & Layout
    float padTop, padBot, padLeft, padRight, gridW, gridH, cellW, cellH;
    sf::VertexArray gridLines; // GPU Geometry for grid
    
    // Initial calculation
    recalculateLayout(cfg, padTop, padBot, padLeft, padRight, gridW, gridH, cellW, cellH);
    updateGridGeometry(gridLines, cfg, padLeft, padTop, gridW, gridH, cellW, cellH);

    // 3. Palette & Tools
    vector<sf::Color> palette = {
        sf::Color(30, 30, 30), sf::Color::White, sf::Color(231, 76, 60), 
        sf::Color(46, 204, 113), sf::Color(52, 152, 219), sf::Color(241, 196, 15), 
        sf::Color(155, 89, 182), sf::Color(230, 126, 34), sf::Color(26, 188, 156), 
        sf::Color(149, 165, 166)
    };

    int currentColorIdx = 1;
    float brushSize = 5.0f;
    bool eraserActive = false;
    bool showSettings = false;
    
    // 4. UI Setup
    vector<Button> buttons;
    float uiX = VIRTUAL_WIDTH - TOOLBAR_WIDTH + 25.0f, uiY = 50.0f;

    for (size_t i = 1; i < palette.size(); i++) {
        buttons.push_back({sf::FloatRect(uiX + ((i-1)%2)*105, uiY + ((i-1)/2)*105, 95, 95), palette[i], "", (int)i});
    }
    uiY += 550.0f;
    buttons.push_back({sf::FloatRect(uiX, uiY, 200, 60), sf::Color(50, 50, 50), "Eraser", -2}); uiY += 80.0f;
    buttons.push_back({sf::FloatRect(uiX, uiY, 95, 60), sf::Color(70, 70, 70), "Size -", -4});
    buttons.push_back({sf::FloatRect(uiX + 105, uiY, 95, 60), sf::Color(70, 70, 70), "Size +", -3}); uiY += 80.0f;
    buttons.push_back({sf::FloatRect(uiX, uiY, 200, 60), sf::Color(192, 57, 43), "CLEAR", -1}); uiY += 80.0f;
    buttons.push_back({sf::FloatRect(uiX, uiY, 95, 60), sf::Color(100, 100, 100), "Undo", -6});
    buttons.push_back({sf::FloatRect(uiX + 105, uiY, 95, 60), sf::Color(100, 100, 100), "Redo", -7}); uiY += 80.0f;
    buttons.push_back({sf::FloatRect(uiX, uiY, 200, 60), sf::Color(41, 128, 185), "Settings", -5});

    // Settings Panel Vars
    float settingsPanelX = 100.0f, settingsPanelY = 100.0f, settingsPanelW = 500.0f, settingsPanelH = 700.0f;
    vector<InputField> inputFields(3);
    inputFields[0] = {sf::FloatRect(settingsPanelX+200, settingsPanelY+60, 100, 40), to_string(cfg.rows), false, 3, true};
    inputFields[1] = {sf::FloatRect(settingsPanelX+200, settingsPanelY+120, 100, 40), to_string(cfg.cols), false, 3, true};
    inputFields[2] = {sf::FloatRect(settingsPanelX+200, settingsPanelY+180, 100, 40), to_string(cfg.baseIndex), false, 1, true};

    vector<Checkbox> checkboxes;
    float cbY = settingsPanelY + 250;
    checkboxes.push_back({sf::FloatRect(settingsPanelX+30, cbY, 25, 25), "Show Top", &cfg.showTop}); cbY+=50;
    checkboxes.push_back({sf::FloatRect(settingsPanelX+30, cbY, 25, 25), "Show Bottom", &cfg.showBottom}); cbY+=50;
    checkboxes.push_back({sf::FloatRect(settingsPanelX+30, cbY, 25, 25), "Show Left", &cfg.showLeft}); cbY+=50;
    checkboxes.push_back({sf::FloatRect(settingsPanelX+30, cbY, 25, 25), "Show Right", &cfg.showRight}); cbY+=50;
    checkboxes.push_back({sf::FloatRect(settingsPanelX+30, cbY, 25, 25), "Cols: L->R", &cfg.colLeftToRight}); cbY+=50;
    checkboxes.push_back({sf::FloatRect(settingsPanelX+30, cbY, 25, 25), "Rows: T->B", &cfg.rowTopToBottom});

    sf::FloatRect applyBtnRect(settingsPanelX+50, settingsPanelY+settingsPanelH-80, 180, 50);
    sf::FloatRect closeBtnRect(settingsPanelX+270, settingsPanelY+settingsPanelH-80, 180, 50);

    // 5. State & Undo/Redo
    bool isDrawing = false;
    sf::Vector2f lastPos;
    bool needsRedraw = true; // Optimization: Only redraw when true
    
    vector<sf::Image> history;
    int historyIndex = -1;
    const int MAX_HISTORY = 50;

    auto saveState = [&]() {
        if (historyIndex < (int)history.size() - 1) 
            history.erase(history.begin() + historyIndex + 1, history.end());
        
        history.push_back(canvas.getTexture().copyToImage());
        if (history.size() > MAX_HISTORY) history.erase(history.begin());
        else historyIndex++;
    };

    auto restoreState = [&](int index) {
        if (index >= 0 && index < (int)history.size()) {
            sf::Texture temp; temp.loadFromImage(history[index]);
            sf::Sprite s(temp);
            canvas.clear(sf::Color(30, 30, 30));
            canvas.draw(s);
            canvas.display();
            historyIndex = index;
            needsRedraw = true;
        }
    };

    saveState(); // Initial state

    // 6. Main Loop
    while (window.isOpen()) {
        sf::Event event;
        
        // OPTIMIZATION: Wait for event instead of polling continuously
        // This blocks execution until an event occurs (0% CPU idle)
        if (window.waitEvent(event)) {
            do {
                if (event.type == sf::Event::Closed) window.close();
                
                if (event.type == sf::Event::Resized) {
                    float ratio = float(window.getSize().x) / float(window.getSize().y);
                    view.setSize(VIRTUAL_HEIGHT * ratio, VIRTUAL_HEIGHT);
                    view.setCenter(VIRTUAL_WIDTH/2, VIRTUAL_HEIGHT/2);
                    window.setView(view);
                    needsRedraw = true;
                }

                if (event.type == sf::Event::KeyPressed) {
                    bool ctrl = sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl);
                    if (ctrl && event.key.code == sf::Keyboard::Z && historyIndex > 0) restoreState(historyIndex - 1);
                    if (ctrl && event.key.code == sf::Keyboard::Y && historyIndex < (int)history.size()-1) restoreState(historyIndex + 1);
                }

                // Text Input
                if (event.type == sf::Event::TextEntered && showSettings) {
                    for (auto& f : inputFields) {
                        if (f.active) {
                            if (event.text.unicode == 8 && !f.value.empty()) f.value.pop_back();
                            else if (event.text.unicode == 13) f.active = false;
                            else if (event.text.unicode < 128) {
                                char c = (char)event.text.unicode;
                                if ((!f.numbersOnly || isdigit(c)) && f.value.length() < f.maxLength) f.value += c;
                            }
                            needsRedraw = true;
                        }
                    }
                }

                // Mouse Handling
                sf::Vector2f mPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                if (event.type == sf::Event::MouseMoved) {
                    if (isDrawing && !showSettings) {
                        sf::Color c = eraserActive ? palette[0] : palette[currentColorIdx];
                        float dist = hypot(mPos.x - lastPos.x, mPos.y - lastPos.y);
                        int steps = max(1, (int)(dist / (brushSize / 4)));
                        
                        for (int i=0; i<=steps; i++) {
                            sf::CircleShape brush(brushSize);
                            brush.setOrigin(brushSize, brushSize);
                            brush.setPosition(lastPos + (mPos - lastPos) * ((float)i/steps));
                            brush.setFillColor(c);
                            canvas.draw(brush);
                        }
                        canvas.display();
                        lastPos = mPos;
                    }
                    needsRedraw = true; // Always redraw on move for brush preview / hover
                }

                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    needsRedraw = true;
                    if (showSettings) {
                        for(auto& f : inputFields) f.active = f.rect.contains(mPos);
                        for(auto& cb : checkboxes) if(cb.rect.contains(mPos)) *cb.valuePtr = !*cb.valuePtr;
                        
                        if (closeBtnRect.contains(mPos)) {
                            inputFields[0].value = to_string(cfg.rows); 
                            inputFields[1].value = to_string(cfg.cols);
                            inputFields[2].value = to_string(cfg.baseIndex);
                            showSettings = false;
                        }
                        if (applyBtnRect.contains(mPos)) {
                            try {
                                int r = stoi(inputFields[0].value), c = stoi(inputFields[1].value), b = stoi(inputFields[2].value);
                                if(r>0 && r<=100 && c>0 && c<=100 && (b==0 || b==1)) {
                                    cfg.rows = r; cfg.cols = c; cfg.baseIndex = b;
                                    recalculateLayout(cfg, padTop, padBot, padLeft, padRight, gridW, gridH, cellW, cellH);
                                    updateGridGeometry(gridLines, cfg, padLeft, padTop, gridW, gridH, cellW, cellH);
                                    saveConfig(cfg);
                                    showSettings = false;
                                }
                            } catch(...) {}
                        }
                    } else {
                        // Main UI
                        if (mPos.x > VIRTUAL_WIDTH - TOOLBAR_WIDTH) {
                            for (auto &b : buttons) {
                                if (b.rect.contains(mPos)) {
                                    if(b.actionID > 0) { currentColorIdx = b.actionID; eraserActive = false; }
                                    else if(b.actionID == -1) { canvas.clear(sf::Color(30,30,30)); canvas.display(); saveState(); }
                                    else if(b.actionID == -2) eraserActive = true;
                                    else if(b.actionID == -3) brushSize = min(50.f, brushSize+2.f);
                                    else if(b.actionID == -4) brushSize = max(2.f, brushSize-2.f);
                                    else if(b.actionID == -5) showSettings = true;
                                    else if(b.actionID == -6 && historyIndex > 0) restoreState(historyIndex-1);
                                    else if(b.actionID == -7 && historyIndex < (int)history.size()-1) restoreState(historyIndex+1);
                                }
                            }
                        } else {
                            isDrawing = true;
                            lastPos = mPos;
                            // Draw initial dot
                            sf::CircleShape brush(brushSize);
                            brush.setOrigin(brushSize, brushSize);
                            brush.setPosition(mPos);
                            brush.setFillColor(eraserActive ? palette[0] : palette[currentColorIdx]);
                            canvas.draw(brush);
                            canvas.display();
                        }
                    }
                }

                if (event.type == sf::Event::MouseButtonReleased) {
                    if (isDrawing) saveState();
                    isDrawing = false;
                    needsRedraw = true;
                }

            } while (window.pollEvent(event)); // Process all pending events
        }

        // --- DRAW (Only if dirty) ---
        if (needsRedraw && window.isOpen()) {
            window.clear(sf::Color(20, 20, 20));

            // 1. Canvas
            sf::Sprite sprite(canvas.getTexture());
            window.draw(sprite);

            // 2. Grid (VertexArray - Fast!)
            window.draw(gridLines);

            // 3. Labels (Still calculated, but only on event)
            sf::Text text; text.setFont(font); text.setCharacterSize(24); text.setFillColor(sf::Color::White);
            for(int r=0; r<cfg.rows; r++) {
                int val = cfg.rowTopToBottom ? (cfg.baseIndex + r) : (cfg.baseIndex + cfg.rows - 1 - r);
                text.setString(to_string(val));
                float y = padTop + r * cellH + cellH/2.f - 15.f;
                if(cfg.showLeft) { text.setPosition(10, y); window.draw(text); }
                if(cfg.showRight) { text.setPosition(VIRTUAL_WIDTH - TOOLBAR_WIDTH - 50, y); window.draw(text); }
            }
            for(int c=0; c<cfg.cols; c++) {
                int val = cfg.colLeftToRight ? (cfg.baseIndex + c) : (cfg.baseIndex + cfg.cols - 1 - c);
                text.setString(to_string(val));
                float x = padLeft + c * cellW + cellW/2.f - 10.f;
                if(cfg.showTop) { text.setPosition(x, 10); window.draw(text); }
                if(cfg.showBottom) { text.setPosition(x, VIRTUAL_HEIGHT - 40); window.draw(text); }
            }

            // 4. Toolbar
            sf::RectangleShape toolbar(sf::Vector2f(TOOLBAR_WIDTH, VIRTUAL_HEIGHT));
            toolbar.setPosition(VIRTUAL_WIDTH - TOOLBAR_WIDTH, 0);
            toolbar.setFillColor(sf::Color(45, 45, 45));
            toolbar.setOutlineColor(sf::Color(100, 100, 100));
            toolbar.setOutlineThickness(-2);
            window.draw(toolbar);

            for (auto &b : buttons) {
                sf::RectangleShape shape(sf::Vector2f(b.rect.width, b.rect.height));
                shape.setPosition(b.rect.left, b.rect.top);
                bool disabled = (b.actionID == -6 && historyIndex <= 0) || (b.actionID == -7 && historyIndex >= (int)history.size()-1);
                shape.setFillColor(disabled ? sf::Color(40,40,40) : b.color);
                
                bool sel = (!eraserActive && b.actionID == currentColorIdx) || (eraserActive && b.actionID == -2);
                shape.setOutlineColor(sel ? sf::Color::White : sf::Color::Black);
                shape.setOutlineThickness(sel ? 4 : 2);
                window.draw(shape);

                if (!b.label.empty()) {
                    sf::Text t; t.setString(b.label); t.setFont(font); t.setCharacterSize(20);
                    t.setFillColor(disabled ? sf::Color(80,80,80) : sf::Color::White);
                    sf::FloatRect tr = t.getLocalBounds();
                    t.setOrigin(tr.left + tr.width/2.f, tr.top + tr.height/2.f);
                    t.setPosition(b.rect.left + b.rect.width/2.f, b.rect.top + b.rect.height/2.f);
                    window.draw(t);
                }
            }

            // Brush Preview
            sf::Text szLbl; szLbl.setString("Size:"); szLbl.setFont(font); szLbl.setCharacterSize(24);
            szLbl.setFillColor(sf::Color::White); szLbl.setPosition(VIRTUAL_WIDTH-TOOLBAR_WIDTH+25, 960);
            window.draw(szLbl);
            sf::CircleShape prev(brushSize); prev.setOrigin(brushSize, brushSize);
            prev.setPosition(VIRTUAL_WIDTH-TOOLBAR_WIDTH+125, 1060);
            prev.setFillColor(eraserActive ? palette[0] : palette[currentColorIdx]);
            window.draw(prev);

            // 5. Settings Modal
            if (showSettings) {
                sf::RectangleShape dim(sf::Vector2f(VIRTUAL_WIDTH, VIRTUAL_HEIGHT));
                dim.setFillColor(sf::Color(0,0,0,180)); window.draw(dim);
                
                sf::RectangleShape p(sf::Vector2f(settingsPanelW, settingsPanelH));
                p.setPosition(settingsPanelX, settingsPanelY); p.setFillColor(sf::Color(50,50,50));
                p.setOutlineColor(sf::Color(100,100,100)); p.setOutlineThickness(3); window.draw(p);

                sf::Text title; title.setString("Settings"); title.setFont(font); title.setCharacterSize(32);
                title.setFillColor(sf::Color::White); title.setPosition(settingsPanelX+20, settingsPanelY+10); window.draw(title);

                vector<string> lbls = {"Rows:", "Cols:", "Base (0/1):"};
                for(size_t i=0; i<3; i++) {
                    sf::Text l; l.setString(lbls[i]); l.setFont(font); l.setCharacterSize(20);
                    l.setFillColor(sf::Color::White); l.setPosition(settingsPanelX+30, inputFields[i].rect.top+8); window.draw(l);
                    
                    sf::RectangleShape fb(sf::Vector2f(inputFields[i].rect.width, inputFields[i].rect.height));
                    fb.setPosition(inputFields[i].rect.left, inputFields[i].rect.top);
                    fb.setFillColor(inputFields[i].active ? sf::Color(80,80,80) : sf::Color(60,60,60));
                    fb.setOutlineColor(inputFields[i].active ? sf::Color::White : sf::Color(100,100,100));
                    fb.setOutlineThickness(2); window.draw(fb);

                    sf::Text val; val.setString(inputFields[i].value + (inputFields[i].active?"_":""));
                    val.setFont(font); val.setCharacterSize(20); val.setFillColor(sf::Color::White);
                    val.setPosition(inputFields[i].rect.left+10, inputFields[i].rect.top+8); window.draw(val);
                }

                for(auto& cb : checkboxes) {
                    sf::RectangleShape bx(sf::Vector2f(cb.rect.width, cb.rect.height));
                    bx.setPosition(cb.rect.left, cb.rect.top); bx.setFillColor(sf::Color(60,60,60));
                    bx.setOutlineColor(sf::Color(100,100,100)); bx.setOutlineThickness(2); window.draw(bx);
                    if(*cb.valuePtr) {
                        sf::RectangleShape chk(sf::Vector2f(15,15)); chk.setPosition(cb.rect.left+5, cb.rect.top+5);
                        chk.setFillColor(sf::Color(46,204,113)); window.draw(chk);
                    }
                    sf::Text l; l.setString(cb.label); l.setFont(font); l.setCharacterSize(20);
                    l.setFillColor(sf::Color::White); l.setPosition(cb.rect.left+35, cb.rect.top+2); window.draw(l);
                }

                sf::RectangleShape ab(sf::Vector2f(applyBtnRect.width, applyBtnRect.height));
                ab.setPosition(applyBtnRect.left, applyBtnRect.top); ab.setFillColor(sf::Color(46,204,113));
                window.draw(ab);
                sf::Text at; at.setString("Apply"); at.setFont(font); at.setCharacterSize(24); at.setFillColor(sf::Color::White);
                at.setPosition(applyBtnRect.left+70, applyBtnRect.top+10); window.draw(at);

                sf::RectangleShape cb(sf::Vector2f(closeBtnRect.width, closeBtnRect.height));
                cb.setPosition(closeBtnRect.left, closeBtnRect.top); cb.setFillColor(sf::Color(192,57,43));
                window.draw(cb);
                sf::Text ct; ct.setString("Cancel"); ct.setFont(font); ct.setCharacterSize(24); ct.setFillColor(sf::Color::White);
                ct.setPosition(closeBtnRect.left+60, closeBtnRect.top+10); window.draw(ct);
            }

            window.display();
            needsRedraw = false;
        }
    }
    return 0;
}
