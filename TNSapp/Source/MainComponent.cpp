#include "MainComponent.h"
#include <algorithm>
#include <cmath>
#include <map>
#include <utility>
#include <vector>

// MainComponent.cpp の既存のインクルードとクラス定義の後に追加

// 色設定用のヘルパーメソッド（更新版）
// 色設定用のヘルパーメソッド（暗めのグレー背景に更新）
void MainComponent::setupComponentColors()
{
    // 背景色を設定（#454545 - 暗めのグレー）
    getLookAndFeel().setColour(juce::ResizableWindow::backgroundColourId, juce::Colour(0x45, 0x45, 0x45));
    
    // ラベルのテキスト色を背景に合わせて変更
    bpmLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    chapterSelectLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    beatVariationLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    paramSetsLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    
    // ステータスラベルの色も調整
    currentPosLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    currentPosLabel.setColour(juce::Label::backgroundColourId, juce::Colour(0x45, 0x45, 0x45).darker(0.1f));
    currentPosLabel.setColour(juce::Label::outlineColourId, juce::Colours::darkgrey);
    
    lastPosLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    lastPosLabel.setColour(juce::Label::backgroundColourId, juce::Colour(0x45, 0x45, 0x45).darker(0.1f));
    lastPosLabel.setColour(juce::Label::outlineColourId, juce::Colours::darkgrey);
    
    allTimeLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    allTimeLabel.setColour(juce::Label::backgroundColourId, juce::Colour(0x45, 0x45, 0x45).darker(0.1f));
    allTimeLabel.setColour(juce::Label::outlineColourId, juce::Colours::darkgrey);
    
    // パラメーターラベルの色も調整（白テキスト）
    for (int i = 0; i < 11; ++i) {
        paramLabels.labels[i].setColour(juce::Label::textColourId, juce::Colours::white);
    }
    
    // スクロールビューの背景色も調整
    viewport.setColour(0x1000000, juce::Colour(0x45, 0x45, 0x45).darker(0.1f));
    
    // BPMスライダーの色調整
    bpmSlider.setColour(juce::Slider::backgroundColourId, juce::Colours::darkgrey);
    bpmSlider.setColour(juce::Slider::trackColourId, juce::Colour(0xd9, 0xd9, 0xd9));
    bpmSlider.setColour(juce::Slider::thumbColourId, juce::Colours::white);
    bpmSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    bpmSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(0x45, 0x45, 0x45).darker(0.1f));
    bpmSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::darkgrey);
}

// MainComponent.cpp内のコンストラクタを修正

MainComponent::MainComponent()
{
    //====================================================================================
    setSize(1500, 820); // 高さも少し大きく設定して下部の黒い部分を解消
    setAudioChannels(0, 2);
    //====================================================================================
    
    // 背景色と関連コンポーネントの色を設定
    setupComponentColors();
    
    // Teenage Engineering風のモダンなデザインスタイル
    auto setupModernButtonStyle = [](juce::Button& button, juce::Colour colour, bool isToggle) {
        button.setColour(juce::TextButton::buttonColourId, colour.withAlpha(0.7f));
        button.setColour(juce::TextButton::buttonOnColourId, colour);
        button.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
        button.setColour(juce::TextButton::textColourOnId, juce::Colours::white);
        
        if (isToggle) {
            button.setClickingTogglesState(true);
        }
    };
    
    // トランスポートコントロールボタンをLookAndFeelに設定
    setupModernButtonStyle(playButton, juce::Colour(40, 200, 100), true);
    playButton.addListener(this);
    playButton.setLookAndFeel(&parameterLookAndFeel); // カスタムLookAndFeelを適用
    addAndMakeVisible(playButton);
    
    setupModernButtonStyle(pauseButton, juce::Colour(200, 60, 60), true);
    pauseButton.addListener(this);
    pauseButton.setLookAndFeel(&parameterLookAndFeel); // カスタムLookAndFeelを適用
    addAndMakeVisible(pauseButton);
    
    setupModernButtonStyle(loopButton, juce::Colour(60, 180, 200), true);
    loopButton.addListener(this);
    loopButton.setLookAndFeel(&parameterLookAndFeel); // カスタムLookAndFeelを適用
    addAndMakeVisible(loopButton);
    
    // ファイル操作ボタンの設定
    setupModernButtonStyle(loadButton, juce::Colour(50, 50, 200), false);
    loadButton.addListener(this);
    loadButton.setLookAndFeel(&parameterLookAndFeel); // カスタムLookAndFeelを適用
    addAndMakeVisible(loadButton);
    
    setupModernButtonStyle(saveButton, juce::Colour(200, 50, 50), false);
    saveButton.addListener(this);
    saveButton.setLookAndFeel(&parameterLookAndFeel); // カスタムLookAndFeelを適用
    saveButton.setName("Save"); // Save ボタンに名前を設定（特別な色を適用するため）
    addAndMakeVisible(saveButton);
    
    setupModernButtonStyle(defaultButton, juce::Colour(100, 100, 100), false);
    defaultButton.addListener(this);
    defaultButton.setLookAndFeel(&parameterLookAndFeel); // カスタムLookAndFeelを適用
    addAndMakeVisible(defaultButton);
    
    setupModernButtonStyle(compileButton, juce::Colour(150, 40, 40), false);
    compileButton.addListener(this);
    compileButton.setLookAndFeel(&parameterLookAndFeel); // カスタムLookAndFeelを適用
    compileButton.setName("Compile"); // Compile ボタンに名前を設定（特別な色を適用するため）
    addAndMakeVisible(compileButton);
    
    // BPMスライダーの設定
    bpmLabel.setFont(juce::Font(16.0f));
    // 注：bpmLabelの色はsetupComponentColorsで設定済み
    addAndMakeVisible(bpmLabel);
    
    bpmSlider.setRange(60.0, 200.0, 1.0);
    bpmSlider.setValue(120.0, juce::dontSendNotification);
    bpmSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    bpmSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 60, 24);
    // 注：bpmSliderの色はsetupComponentColorsで設定済み
    bpmSlider.addListener(this);
    addAndMakeVisible(bpmSlider);
    
    // プログレスバーを非表示に設定
    progressBar.setVisible(false);
    addAndMakeVisible(progressBar);
    
    // 表示用ラベルの設定
    auto setupDisplayLabel = [](juce::Label& label, juce::Colour colour) {
        label.setFont(juce::Font(16.0f));
        label.setJustificationType(juce::Justification::centredLeft);
    };
    
    setupDisplayLabel(currentPosLabel, juce::Colours::yellow);
    currentPosLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(currentPosLabel);
    
    setupDisplayLabel(lastPosLabel, juce::Colours::yellow);
    lastPosLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(lastPosLabel);
    
    setupDisplayLabel(allTimeLabel, juce::Colours::yellow);
    allTimeLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(allTimeLabel);
    
    // チャプターセレクトの設定
    chapterSelectLabel.setFont(juce::Font(16.0f));
    // 注：chapterSelectLabelの色はsetupComponentColorsで設定済み
    addAndMakeVisible(chapterSelectLabel);
    
    // チャプターラジオボタン設定 - 関数定義を修正
    auto setupChapterButton = [this](juce::ToggleButton& button, int groupId) {
        button.setRadioGroupId(groupId, juce::dontSendNotification);
        button.setClickingTogglesState(true);
        button.setLookAndFeel(&parameterLookAndFeel); // カスタムLookAndFeelを適用
        button.addListener(this);
        addAndMakeVisible(button);
    };
    
    setupChapterButton(chapterBtn1, 1);
    chapterBtn1.setToggleState(true, juce::dontSendNotification);
    
    setupChapterButton(chapterBtn2, 1);
    setupChapterButton(chapterBtn3, 1);
    setupChapterButton(chapterBtn4, 1);
    
    // ビートバリエーションの設定
    beatVariationLabel.setFont(juce::Font(16.0f));
    // 注：beatVariationLabelの色はsetupComponentColorsで設定済み
    addAndMakeVisible(beatVariationLabel);
    
    // ビートバリエーションラジオボタン設定
    setupChapterButton(beatVariationBtn1, 2);
    beatVariationBtn1.setToggleState(true, juce::dontSendNotification);
    
    setupChapterButton(beatVariationBtn2, 2);
    
    // インバートコントロールの設定も同様に修正
    auto setupToggleButton = [this](juce::ToggleButton& button, juce::Colour colour) {
        button.setLookAndFeel(&parameterLookAndFeel); // カスタムLookAndFeelを適用
        button.addListener(this);
        addAndMakeVisible(button);
    };
    
    setupToggleButton(beatInvertButton, juce::Colour(240, 200, 40));
    setupToggleButton(arpeggioInvertButton, juce::Colour(240, 200, 40));
    setupToggleButton(panInvertButton, juce::Colour(240, 200, 40));
    
    // ソロコントロールの設定
    setupToggleButton(stringSoloButton, juce::Colour(100, 220, 50));
    stringSoloButton.setToggleState(true, juce::dontSendNotification);
    
    setupToggleButton(pianoSoloButton, juce::Colour(100, 220, 50));
    pianoSoloButton.setToggleState(true, juce::dontSendNotification);
    
    // パラメーターセットパネルの作成と設定
    auto setupRadioButtonPanel = [this](RadioButtonPanel& panel) {
        panel.titleLabel.setFont(juce::Font(14.0f));
        // 注：titleLabelの色はsetupComponentColorsで設定済み
        panel.titleLabel.setJustificationType(juce::Justification::centred);
        
        // Resetボタンを非表示に設定
        panel.resetButton.setVisible(false);
        
        for (int i = 0; i < 11; ++i)
        {
            panel.buttons[i].setRadioGroupId(100 + (&panel - &sets1Panel), juce::dontSendNotification);
            // ボタンテキストを番号に設定（オプション）
            panel.buttons[i].setButtonText(juce::String(i + 1));
            panel.buttons[i].setClickingTogglesState(true);
            
            // カスタムLookAndFeelを適用
            panel.buttons[i].setLookAndFeel(&parameterLookAndFeel);
            
            // イベントリスナー設定
            panel.buttons[i].addListener(this);
        }
    };
    
    setupRadioButtonPanel(sets1Panel);
    setupRadioButtonPanel(sets2Panel);
    setupRadioButtonPanel(sets3Panel);
    setupRadioButtonPanel(sets4Panel);
    setupRadioButtonPanel(sets5Panel);
    setupRadioButtonPanel(sets6Panel);
    setupRadioButtonPanel(sets7Panel);
    setupRadioButtonPanel(sets8Panel);
    setupRadioButtonPanel(sets9Panel);
    setupRadioButtonPanel(sets10Panel);
    setupRadioButtonPanel(sets11Panel);
    
    // パラメーターラベルの設定
    for (int i = 0; i < 11; ++i)
    {
        paramLabels.labels[i].setFont(juce::Font(14.0f));
        // 注：labels[i]の色はsetupComponentColorsで設定済み
    }
    
    // コンストラクタ内のスクロールビュー設定部分 (MainComponent.cpp)
    // スクロールビューに追加
    const int rowHeight = 45;
    const int totalRows = 11;
    const int headerHeight = 50;
    const int footerSpace = 50;
    // 元のcontentHeightから100px縮小
    const int contentHeight = headerHeight + (totalRows * rowHeight) + footerSpace - 700;


    // パラメーターコンポーネントの作成
    this->parametersComponent = new juce::Component();
    // パラメーターコンポーネントのサイズを設定（幅は広めに、高さは縮小）
    parametersComponent->setSize(1500, contentHeight);

    // パラメーターラベルをコンポーネントに追加
    for (int i = 0; i < 11; ++i)
    {
        parametersComponent->addAndMakeVisible(paramLabels.labels[i]);
    }

    // 各パネルコンポーネントを追加
    auto addPanelToComponent = [this](RadioButtonPanel& panel) {
        if (this->parametersComponent != nullptr) {
            this->parametersComponent->addAndMakeVisible(panel.titleLabel);
            
            // Resetボタンが非表示設定の場合は追加不要
            if (panel.resetButton.isVisible()) {
                this->parametersComponent->addAndMakeVisible(panel.resetButton);
            }
            
            for (int i = 0; i < 11; ++i)
                this->parametersComponent->addAndMakeVisible(panel.buttons[i]);
        }
    };

    // すべてのパネルを追加
    addPanelToComponent(sets1Panel);
    addPanelToComponent(sets2Panel);
    addPanelToComponent(sets3Panel);
    addPanelToComponent(sets4Panel);
    addPanelToComponent(sets5Panel);
    addPanelToComponent(sets6Panel);
    addPanelToComponent(sets7Panel);
    addPanelToComponent(sets8Panel);
    addPanelToComponent(sets9Panel);
    addPanelToComponent(sets10Panel);
    addPanelToComponent(sets11Panel);

    // ビューポート設定
    viewport.setViewedComponent(parametersComponent, true);
    viewport.setScrollBarsShown(true, false); // スクロールバー表示設定 - 縦スクロールのみ有効
    // ビューポート設定の追加
    viewport.setScrollBarThickness(12); // スクロールバーの太さを設定
    // メインコンポーネントに追加
    addAndMakeVisible(viewport);

    // タイトルラベル
    paramSetsLabel.setText("Parameter Sets:", juce::dontSendNotification);
    paramSetsLabel.setFont(juce::Font(18.0f));
    addAndMakeVisible(paramSetsLabel);
    
    juce::Timer::callAfterDelay(500, [this]() {
        pd.sendBang("juce-default");
        pd.sendBang("juce-compile");
        DBG("Sent default command to PD at startup");
    });
}

// MainComponent::~MainComponent() を更新して、新しくLookAndFeelを適用したボタンの参照を解除

MainComponent::~MainComponent()
{
    // トランスポートボタンからLookAndFeelの参照を解除
    playButton.setLookAndFeel(nullptr);
    pauseButton.setLookAndFeel(nullptr);
    loopButton.setLookAndFeel(nullptr);
    
    // ファイル操作ボタンからLookAndFeelの参照を解除
    loadButton.setLookAndFeel(nullptr);
    saveButton.setLookAndFeel(nullptr);
    defaultButton.setLookAndFeel(nullptr);
    compileButton.setLookAndFeel(nullptr);
    
    // チャプターボタンからLookAndFeelの参照を解除
    chapterBtn1.setLookAndFeel(nullptr);
    chapterBtn2.setLookAndFeel(nullptr);
    chapterBtn3.setLookAndFeel(nullptr);
    chapterBtn4.setLookAndFeel(nullptr);
    
    // ビートバリエーションボタンからLookAndFeelの参照を解除
    beatVariationBtn1.setLookAndFeel(nullptr);
    beatVariationBtn2.setLookAndFeel(nullptr);
    
    // インバートボタンからLookAndFeelの参照を解除
    beatInvertButton.setLookAndFeel(nullptr);
    arpeggioInvertButton.setLookAndFeel(nullptr);
    panInvertButton.setLookAndFeel(nullptr);
    
    // ソロボタンからLookAndFeelの参照を解除
    stringSoloButton.setLookAndFeel(nullptr);
    pianoSoloButton.setLookAndFeel(nullptr);
    
    // パラメーターセットパネルのボタンからLookAndFeelの参照を解除
    auto removeCustomLookAndFeel = [](RadioButtonPanel& panel) {
        for (int i = 0; i < 11; ++i)
        {
            panel.buttons[i].setLookAndFeel(nullptr);
        }
    };
    
    removeCustomLookAndFeel(sets1Panel);
    removeCustomLookAndFeel(sets2Panel);
    removeCustomLookAndFeel(sets3Panel);
    removeCustomLookAndFeel(sets4Panel);
    removeCustomLookAndFeel(sets5Panel);
    removeCustomLookAndFeel(sets6Panel);
    removeCustomLookAndFeel(sets7Panel);
    removeCustomLookAndFeel(sets8Panel);
    removeCustomLookAndFeel(sets9Panel);
    removeCustomLookAndFeel(sets10Panel);
    removeCustomLookAndFeel(sets11Panel);
    
    //====================================================================================
    shutdownAudio();
    //====================================================================================
}


void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    // 既存のリソースをクリーンアップ（安全のため）
    releaseResources();
    
    //====================================================================================
    const int inputChannels = 0;
    const int outputChannels = 2;
    
    try {
        pd.init(inputChannels, outputChannels, static_cast<int>(sampleRate));
        pd.computeAudio(true);
        pd.setReceiver(this);
        
        auto appBundle = juce::File::getSpecialLocation(juce::File::currentApplicationFile);
        auto pdFile = appBundle.getChildFile("Contents")
            .getChildFile("Resources").getChildFile("main.pd");
        
        DBG("App Bundle Path: " << pdFile.getFullPathName());
        
        if (!pdFile.existsAsFile()) {
            DBG("Patch file not found: " + pdFile.getFullPathName().toStdString());
            return;
        }
        //pdを開く、外部ライブラリの読み込み
        pd.openPatch("main.pd", pdFile.getParentDirectory().getFullPathName().toStdString());
        pd.addToSearchPath(pdFile.getParentDirectory().getFullPathName().toStdString());
        
        // absフォルダをサーチパスに追加
        auto absDir = pdFile.getParentDirectory().getChildFile("abs");
        if (absDir.isDirectory()) {
            pd.addToSearchPath(absDir.getFullPathName().toStdString());
        }

        // sfzフォルダ（サンプル音源）をサーチパスに追加
        auto sfzDir = pdFile.getParentDirectory().getChildFile("sfz");
        if (sfzDir.isDirectory()) {
            pd.addToSearchPath(sfzDir.getFullPathName().toStdString());
        }
        
        //====================================================================================
        
        // テキストファイル操作用のメッセージ登録
        pd.subscribe("text-save-complete");
        pd.subscribe("text-load-complete");
        
        // ベースディレクトリの設定
        defaultDirectory = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory)
            .getChildFile("TNSapp");
        
        // すべての必要なディレクトリを確認・作成
        juce::StringArray directories;
        directories.add("presets");
        directories.add("save");
        directories.add("backup");
        
        // ベースディレクトリの確認
        if (!defaultDirectory.exists())
        {
            auto result = defaultDirectory.createDirectory();
            if (result.failed())
            {
                DBG("Failed to create base directory: " + defaultDirectory.getFullPathName() +
                    " Error: " + result.getErrorMessage());
            }
            else
            {
                DBG("Created base directory: " + defaultDirectory.getFullPathName());
            }
        }
        
        // すべてのサブディレクトリを確認・作成
        for (auto& dirName : directories)
        {
            juce::File subDir = defaultDirectory.getChildFile(dirName);
            if (!subDir.exists())
            {
                auto result = subDir.createDirectory();
                if (result.failed())
                {
                    DBG("Failed to create directory: " + subDir.getFullPathName() +
                        " Error: " + result.getErrorMessage());
                }
                else
                {
                    DBG("Created directory: " + subDir.getFullPathName());
                }
            }
        }
        
        // パスをPureDataに送信
        pd.sendSymbol("juce-base-dir", defaultDirectory.getFullPathName().toStdString());
        pd.sendSymbol("juce-presets-dir", defaultDirectory.getChildFile("presets").getFullPathName().toStdString());
        pd.sendSymbol("juce-save-dir", defaultDirectory.getChildFile("save").getFullPathName().toStdString());
        
        DBG("Directory paths sent to PureData");
        
        // ファイル操作関連のメッセージを登録
        pd.subscribe("juce-file-path");
        pd.subscribe("juce-do-load");
        pd.subscribe("pd-load-start");
        pd.subscribe("pd-load-complete");
        
        // メッセージ登録も追加
        pd.subscribe("pd-save-complete");
        pd.subscribe("pd-load-complete");
        
        // メッセージの送受信を明示的に登録
        pd.subscribe("playbutton");
        pd.subscribe("setpause");
        pd.subscribe("setloop");
        pd.subscribe("loadBPM");
        pd.subscribe("countpos");
        pd.subscribe("lastpos");
        pd.subscribe("alltime");
        pd.subscribe("loadchapter");
        pd.subscribe("loadbeat");
        pd.subscribe("setbeatinvert");
        pd.subscribe("setarpeggioinvert");
        pd.subscribe("setpaninvert");
        pd.subscribe("string-state");
        pd.subscribe("piano-state");
        
        // パラメーターセット用のサブスクリプション
        pd.subscribe("sets1");
        pd.subscribe("sets2");
        pd.subscribe("sets3");
        pd.subscribe("sets4");
        pd.subscribe("sets5");
        pd.subscribe("sets6");
        pd.subscribe("sets7");
        pd.subscribe("sets8");
        pd.subscribe("sets9");
        pd.subscribe("sets10");
        pd.subscribe("sets11");
        
        // BPMのみ初期値を送信（他はPureDataから受信する）
        pd.sendFloat("juce-bpm", 120.0f);
        
        // 各値のクエリを送信して現在の状態を取得
        pd.sendBang("juce-query-countpos");
        pd.sendBang("juce-query-lastpos");
        pd.sendBang("juce-query-alltime");
    }
    catch (const std::exception& e) {
        DBG("Error in prepareToPlay: " << e.what());
    }
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    if (bufferToFill.buffer == nullptr || bufferToFill.numSamples <= 0)
        return;
    
    try {
        //====================================================================================
        float* outL = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
        float* outR = bufferToFill.buffer->getNumChannels() > 1
        ? bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample)
        : nullptr;
        
        const int blockSize = bufferToFill.numSamples;
        const int ticks = blockSize / pd.blockSize();
        
        std::vector<float> output(blockSize * 2, 0.0f);
        pd.processFloat(ticks, nullptr, output.data());
        
        for (int i = 0; i < blockSize; ++i) {
            outL[i] = output[i * 2];
            if (outR) outR[i] = output[i * 2 + 1];
        }
        //====================================================================================
    }
    catch (const std::exception& e) {
        DBG("Error in getNextAudioBlock: " << e.what());
        bufferToFill.clearActiveBufferRegion();
    }
}

void MainComponent::releaseResources()
{
    //====================================================================================
    try {
        pd.computeAudio(false);
        // 全てのサブスクリプションを解除
        pd.unsubscribeAll();
    }
    catch (const std::exception& e) {
        DBG("Error in releaseResources: " << e.what());
    }
    //====================================================================================
}

void MainComponent::paint(juce::Graphics& g)
{
    // 背景色を #454545（暗めのグレー）に設定
    g.fillAll(juce::Colour(0x45, 0x45, 0x45));
    
    // 下部に軽いセパレーターライン（オプション）
    g.setColour(juce::Colours::black.withAlpha(0.3f));
    g.drawLine(0.0f, static_cast<float>(getHeight() - 100),
               static_cast<float>(getWidth()), static_cast<float>(getHeight() - 100),
               1.0f);
    
    // オプション：セクション間の区切り線をより洗練された形で追加
    g.setColour(juce::Colours::black.withAlpha(0.2f));
    g.drawLine(0.0f, 180.0f, static_cast<float>(getWidth()), 180.0f, 1.0f);
}

void MainComponent::resized()
{
    // サイズが有効かどうかチェック
    auto bounds = getLocalBounds();
    if (bounds.getWidth() <= 0 || bounds.getHeight() <= 0)
        return;
    
    // リサイズしても比率を維持
    const double aspectRatio = 1500.0 / 820.0;
    int newWidth = bounds.getWidth();
    int newHeight = bounds.getHeight();
    
    // 画面サイズの比率に基づいてパラメーターコンポーネントを調整
    if (parametersComponent != nullptr) {
        parametersComponent->setSize(newWidth, 650 * newHeight / 820);
    }
    
    // より大きな余白を設定
    const int margin = 45; // ウィンドウの余白を大きく
    auto area = bounds.reduced(margin);
    
    // 各コンポーネントのサイズが負にならないように注意
    if (area.getWidth() <= 0 || area.getHeight() <= 0)
        return;
    
    // ボタンの間隔を設定
    int transportButtonSpacing = 12;  // ボタン間の間隔
    int groupSpacing = 40;            // グループ間の間隔
    
    // 画像に合わせてトランスポートボタンのサイズを設定 (100x50px)
    int transportButtonWidth = 95;    // 少しサイズダウン
    int transportButtonHeight = 48;   // 少しサイズダウン
    
    // トランスポートコントロールの配置（上部に3つのグループに分けて配置）
    auto topArea = area.removeFromTop(transportButtonHeight + 20);
    
    // グループ1：Play/Pause/Loop（左側）
    auto totalWidth1 = (transportButtonWidth * 3) + (transportButtonSpacing * 2);
    auto leftGroupArea = topArea.removeFromLeft(totalWidth1);
    
    playButton.setBounds(leftGroupArea.removeFromLeft(transportButtonWidth));
    leftGroupArea.removeFromLeft(transportButtonSpacing);
    pauseButton.setBounds(leftGroupArea.removeFromLeft(transportButtonWidth));
    leftGroupArea.removeFromLeft(transportButtonSpacing);
    loopButton.setBounds(leftGroupArea);
    
    // グループ2：Load/Save（左右中央寄り）
    // 中央よりやや左側に配置
    auto totalWidth2 = (transportButtonWidth * 2) + transportButtonSpacing;
    int centerStartX = (topArea.getWidth() - totalWidth2) / 3; // 3で割って左寄りに
    
    auto centerGroupArea = topArea.removeFromLeft(centerStartX + totalWidth2).withTrimmedLeft(centerStartX);
    
    loadButton.setBounds(centerGroupArea.removeFromLeft(transportButtonWidth));
    centerGroupArea.removeFromLeft(transportButtonSpacing);
    saveButton.setBounds(centerGroupArea);
    
    // グループ3：Default（右側）
    auto defaultArea = topArea.removeFromRight(transportButtonWidth + 20);
    defaultButton.setBounds(defaultArea.removeFromRight(transportButtonWidth));
    
    // グループ4：Compile（Defaultの少し左）
    auto compileWidth = transportButtonWidth;
    auto compileArea = topArea.removeFromRight(compileWidth + groupSpacing); // グループ間隔を広く
    compileButton.setBounds(compileArea.removeFromLeft(compileWidth));
    
    // ボタンに特別な名前を設定（LookAndFeelで識別するため）
    saveButton.setName("Save");
    compileButton.setName("Compile");
    
    // BPM設定エリアと状態表示エリア
    area.removeFromTop(25); // 少し間隔を空ける
    auto infoArea = area.removeFromTop(30); // 高さを揃える（小さく）
    
    // BPMエリアとステータスエリアを分離
    auto bpmArea = infoArea.removeFromLeft(infoArea.getWidth() / 3 - 50); // 左側1/3より少し小さく
    bpmLabel.setBounds(bpmArea.removeFromLeft(60));
    bpmSlider.setBounds(bpmArea);
    
    // ステータスエリア - 中央寄りに配置
    auto statusAreaWidth = infoArea.getWidth() - 50; // 右側に少し余裕
    auto statusArea = infoArea.withSizeKeepingCentre(statusAreaWidth, infoArea.getHeight());
    
    // プログレスバーを非表示に
    progressBar.setVisible(false);
    
    // ステータスラベル - プログレスバーの位置に直接配置
    auto labelWidth = statusArea.getWidth() / 3;
    currentPosLabel.setBounds(statusArea.getX(), statusArea.getY(), labelWidth, statusArea.getHeight());
    lastPosLabel.setBounds(statusArea.getX() + labelWidth, statusArea.getY(), labelWidth, statusArea.getHeight());
    allTimeLabel.setBounds(statusArea.getX() + 2 * labelWidth, statusArea.getY(), labelWidth, statusArea.getHeight());
    
    // チャプターセレクトとビートバリエーション - 左右に分散配置
    area.removeFromTop(25); // 少し間隔を空ける
    
    // セレクションエリア全体のボックス
    auto selectionBoxHeight = 85; // ラベルとボタンの間隔を広げるため高さを大きく
    auto selectionArea = area.removeFromTop(selectionBoxHeight);
    
    // 3つのグループ分けエリア（左右に均等配置）
    auto groupWidth = selectionArea.getWidth() / 3;
    
    // Chapter Select グループ（左側1/3）
    auto chapterGroupArea = selectionArea.removeFromLeft(groupWidth).reduced(5, 0);
    
    // Chapter Selectのラベル
    auto chapterLabelArea = chapterGroupArea.removeFromTop(25);
    chapterSelectLabel.setBounds(chapterLabelArea);
    
    // ラベルとボタンの間に間隔を設ける
    chapterGroupArea.removeFromTop(15); // 15ピクセル空ける
    
    // 画像に合わせてチャプターボタンのサイズを設定 (80x40px)
    int chapterButtonWidth = 75;  // 少しサイズダウン
    int chapterButtonHeight = 38; // 少しサイズダウン
    int chapterButtonSpacing = 12; // 間隔を広げる
    
    // ボタンの合計幅を計算
    auto totalChapterButtonWidth = (chapterButtonWidth * 4) + (chapterButtonSpacing * 3);
    
    // センタリングのための左マージン計算
    auto chapterButtonsLeftMargin = (chapterGroupArea.getWidth() - totalChapterButtonWidth) / 2;
    if (chapterButtonsLeftMargin < 0) chapterButtonsLeftMargin = 0;
    
    // チャプターボタンを配置 - 横に並べて中央寄せ
    auto chapterButtonsArea = chapterGroupArea.removeFromLeft(chapterGroupArea.getWidth());
    chapterButtonsArea.removeFromLeft(chapterButtonsLeftMargin);
    
    chapterBtn1.setBounds(chapterButtonsArea.getX(), chapterButtonsArea.getY(),
                         chapterButtonWidth, chapterButtonHeight);
    chapterBtn2.setBounds(chapterButtonsArea.getX() + chapterButtonWidth + chapterButtonSpacing,
                         chapterButtonsArea.getY(),
                         chapterButtonWidth, chapterButtonHeight);
    chapterBtn3.setBounds(chapterButtonsArea.getX() + (chapterButtonWidth + chapterButtonSpacing) * 2,
                         chapterButtonsArea.getY(),
                         chapterButtonWidth, chapterButtonHeight);
    chapterBtn4.setBounds(chapterButtonsArea.getX() + (chapterButtonWidth + chapterButtonSpacing) * 3,
                         chapterButtonsArea.getY(),
                         chapterButtonWidth, chapterButtonHeight);
    
    // Beat Variation グループ（中央1/3）
    auto beatGroupArea = selectionArea.removeFromLeft(groupWidth).reduced(5, 0);
    
    // Beat Variationのラベル
    auto beatLabelArea = beatGroupArea.removeFromTop(25);
    beatVariationLabel.setBounds(beatLabelArea);
    
    // ラベルとボタンの間に間隔を設ける
    beatGroupArea.removeFromTop(15); // 15ピクセル空ける
    
    // ビートバリエーションボタンの合計幅を計算
    auto totalBeatButtonWidth = (chapterButtonWidth * 2) + chapterButtonSpacing;
    
    // センタリングのための左マージン計算
    auto beatButtonsLeftMargin = (beatGroupArea.getWidth() - totalBeatButtonWidth) / 2;
    if (beatButtonsLeftMargin < 0) beatButtonsLeftMargin = 0;
    
    // Beat Variationボタン配置 - 横に並べて中央寄せ
    auto beatButtonsArea = beatGroupArea.removeFromLeft(beatGroupArea.getWidth());
    beatButtonsArea.removeFromLeft(beatButtonsLeftMargin);
    
    beatVariationBtn1.setBounds(beatButtonsArea.getX(), beatButtonsArea.getY(),
                               chapterButtonWidth, chapterButtonHeight);
    beatVariationBtn2.setBounds(beatButtonsArea.getX() + chapterButtonWidth + chapterButtonSpacing,
                               beatButtonsArea.getY(),
                               chapterButtonWidth, chapterButtonHeight);
    
    // Invert系ボタン グループ（右側1/3）
    auto invertGroupArea = selectionArea.reduced(5, 0);
    
    // インバートボタンはラベルがないので上部に余白を設ける
    invertGroupArea.removeFromTop(25);
    
    // ラベルとボタンの間に間隔を設ける
    invertGroupArea.removeFromTop(15); // 15ピクセル空ける
    
    // インバートボタンのサイズ設定
    int invertButtonWidth = 75;  // 少しサイズダウン
    int invertButtonHeight = 38; // 少しサイズダウン
    
    // インバートボタンの合計幅を計算
    auto totalInvertButtonWidth = (invertButtonWidth * 3) + (chapterButtonSpacing * 2);
    
    // センタリングのための左マージン計算
    auto invertButtonsLeftMargin = (invertGroupArea.getWidth() - totalInvertButtonWidth) / 2;
    if (invertButtonsLeftMargin < 0) invertButtonsLeftMargin = 0;
    
    // インバートボタン配置 - 横に並べて中央寄せ
    auto invertButtonsArea = invertGroupArea.removeFromLeft(invertGroupArea.getWidth());
    invertButtonsArea.removeFromLeft(invertButtonsLeftMargin);
    
    beatInvertButton.setBounds(invertButtonsArea.getX(), invertButtonsArea.getY(),
                              invertButtonWidth, invertButtonHeight);
    arpeggioInvertButton.setBounds(invertButtonsArea.getX() + invertButtonWidth + chapterButtonSpacing,
                                  invertButtonsArea.getY(),
                                  invertButtonWidth, invertButtonHeight);
    panInvertButton.setBounds(invertButtonsArea.getX() + (invertButtonWidth + chapterButtonSpacing) * 2,
                             invertButtonsArea.getY(),
                             invertButtonWidth, invertButtonHeight);
    
    // パラメーターセットエリア
    area.removeFromTop(20); // 少し間隔を空ける
    auto paramLabelArea = area.removeFromTop(30);
    paramSetsLabel.setBounds(paramLabelArea.withSizeKeepingCentre(150, 20));
    
    // ソロボタンをメインコンポーネントに配置 - viewport下部に固定、ボタンサイズを拡大
    int soloButtonY = bounds.getHeight() - margin - 40; // 下部マージンから40px上
    int soloButtonWidth = 115;  // 幅を少し小さく
    int soloButtonHeight = 38;  // 高さを少し小さく
    
    auto soloButtonsArea = bounds.removeFromBottom(soloButtonHeight + margin).reduced(margin, 0);
    soloButtonsArea.setY(soloButtonY - soloButtonHeight/2); // 位置を明示的に設定
    
    // ソロボタンを左右に分散配置
    auto leftSoloArea = soloButtonsArea.removeFromLeft(soloButtonsArea.getWidth() / 2 - 15); // 余白を増やす
    auto rightSoloArea = soloButtonsArea;
    
    stringSoloButton.setBounds(leftSoloArea.withSizeKeepingCentre(soloButtonWidth, soloButtonHeight));
    pianoSoloButton.setBounds(rightSoloArea.withSizeKeepingCentre(soloButtonWidth, soloButtonHeight));

    // スクロールビュー - ソロボタンの上に配置
    auto viewportArea = area;
    viewportArea.setBottom(soloButtonY - 25); // ソロボタンとの間隔を広げる
    viewport.setBounds(viewportArea);
    
    // パラメーターコンポーネントの設定
    if (parametersComponent != nullptr) {
        const int rowHeight = 42 * bounds.getHeight() / 820; // 少し小さく
        const int labelWidth = 115 * bounds.getWidth() / 1500;
        
        // ボタンサイズを更新 - パラメータセットボタンは50x30px
        const int buttonWidth = 48;  // 少し小さく
        const int buttonHeight = 28; // 少し小さく
        
        const int columnWidth = 115 * bounds.getWidth() / 1500;
        const int spacing = 12 * bounds.getWidth() / 1500; // 間隔を広く
        
        // パラメーターラベルの配置
        for (int i = 0; i < 11; ++i) {
            paramLabels.labels[i].setBounds(10, i * rowHeight + 50, labelWidth, rowHeight);
        }
        
        // パネルの配置
        auto configurePanel = [rowHeight, labelWidth, buttonWidth, buttonHeight, columnWidth, spacing, bounds](RadioButtonPanel& panel, int panelIndex) {
            // カラム幅を少し広くする
            const int adjustedColumnWidth = panelIndex == 10 ? columnWidth + 30 * bounds.getWidth() / 1500 : columnWidth; // Articulationの選択用に幅を広げる
            int xPos = labelWidth + columnWidth * panelIndex + spacing;
            
            // パネルタイトル - Articulationの選択のタイトルはもう少し幅を広く
            panel.titleLabel.setBounds(xPos, 10, adjustedColumnWidth - 10, 30);
            
            // Resetボタンは非表示設定されているため、配置しない
            if (panel.resetButton.isVisible()) {
                panel.resetButton.setBounds(xPos + adjustedColumnWidth - 30, 10, 20, 20);
            }
            
            // パネルのボタン - 48x28px、縦に配置
            for (int i = 0; i < 11; ++i) {
                panel.buttons[i].setBounds(xPos + (adjustedColumnWidth - buttonWidth) / 2,
                                         i * rowHeight + 50,
                                         buttonWidth,
                                         buttonHeight);
            }
        };
        
        configurePanel(sets1Panel, 0);
        configurePanel(sets2Panel, 1);
        configurePanel(sets3Panel, 2);
        configurePanel(sets4Panel, 3);
        configurePanel(sets5Panel, 4);
        configurePanel(sets6Panel, 5);
        configurePanel(sets7Panel, 6);
        configurePanel(sets8Panel, 7);
        configurePanel(sets9Panel, 8);
        configurePanel(sets10Panel, 9);
        configurePanel(sets11Panel, 10);
    }
    
    // 初期状態でビューポートを上部にスクロール
    viewport.setViewPosition(30, 0);
}

void MainComponent::buttonClicked(juce::Button* button)
{
    try {
        // トランスポートコントロール
        if (button == &playButton) {
            pd.sendFloat("juce-play", playButton.getToggleState() ? 1.0f : 0.0f);
        }
        else if (button == &pauseButton) {
            pd.sendFloat("juce-pause", pauseButton.getToggleState() ? 1.0f : 0.0f);
        }
        else if (button == &loopButton) {
            pd.sendFloat("juce-loop", loopButton.getToggleState() ? 1.0f : 0.0f);
        }
        else if (button == &saveButton) {
            //pd.sendBang("juce-save"); // 既存のPd処理を維持
            saveTextFile();           // テキストファイル保存を追加
        }
        // 読み込みボタン処理
        else if (button == &loadButton) {
            //pd.sendBang("juce-load"); // 既存のPd処理を維持
            loadTextFile();           // テキストファイル読み込みを追加
        }
        else if (button == &defaultButton) {
            pd.sendBang("juce-default");
        }
        else if (button == &compileButton) {
            pd.sendBang("juce-compile");
        }
        // チャプターセレクト
        else if (button == &chapterBtn1 && chapterBtn1.getToggleState()) {
            pd.sendFloat("juce-loadchapter", 0.0f);
        }
        else if (button == &chapterBtn2 && chapterBtn2.getToggleState()) {
            pd.sendFloat("juce-loadchapter", 1.0f);
        }
        else if (button == &chapterBtn3 && chapterBtn3.getToggleState()) {
            pd.sendFloat("juce-loadchapter", 2.0f);
        }
        else if (button == &chapterBtn4 && chapterBtn4.getToggleState()) {
            pd.sendFloat("juce-loadchapter", 3.0f);
        }
        // ビートバリエーション
        else if (button == &beatVariationBtn1 && beatVariationBtn1.getToggleState()) {
            pd.sendFloat("juce-loadbeat", 0.0f);
        }
        else if (button == &beatVariationBtn2 && beatVariationBtn2.getToggleState()) {
            pd.sendFloat("juce-loadbeat", 1.0f);
        }
        // インバートコントロール
        else if (button == &beatInvertButton) {
            pd.sendFloat("juce-setbeatinvert", beatInvertButton.getToggleState() ? 1.0f : 0.0f);
        }
        else if (button == &arpeggioInvertButton) {
            pd.sendFloat("juce-setarpeggioinvert", arpeggioInvertButton.getToggleState() ? 1.0f : 0.0f);
        }
        else if (button == &panInvertButton) {
            pd.sendFloat("juce-setpaninvert", panInvertButton.getToggleState() ? 1.0f : 0.0f);
        }
        // ソロコントロール
        else if (button == &stringSoloButton) {
            pd.sendFloat("juce-string-state", stringSoloButton.getToggleState() ? 1.0f : 0.0f);
        }
        else if (button == &pianoSoloButton) {
            pd.sendFloat("juce-piano-state", pianoSoloButton.getToggleState() ? 1.0f : 0.0f);
        }
        // パラメーターセットボタン
        else {
            // Resetボタンは非表示にしているので必要なし
            
            // ラジオボタン (sets1 ~ sets11)
            for (int i = 0; i < 11; ++i) {
                if (sets1Panel.buttons[i].getToggleState() && button == &sets1Panel.buttons[i]) {
                    pd.sendFloat("juce-sets1", static_cast<float>(i));
                    break;
                }
                else if (sets2Panel.buttons[i].getToggleState() && button == &sets2Panel.buttons[i]) {
                    pd.sendFloat("juce-sets2", static_cast<float>(i));
                    break;
                }
                else if (sets3Panel.buttons[i].getToggleState() && button == &sets3Panel.buttons[i]) {
                    pd.sendFloat("juce-sets3", static_cast<float>(i));
                    break;
                }
                else if (sets4Panel.buttons[i].getToggleState() && button == &sets4Panel.buttons[i]) {
                    pd.sendFloat("juce-sets4", static_cast<float>(i));
                    break;
                }
                else if (sets5Panel.buttons[i].getToggleState() && button == &sets5Panel.buttons[i]) {
                    pd.sendFloat("juce-sets5", static_cast<float>(i));
                    break;
                }
                else if (sets6Panel.buttons[i].getToggleState() && button == &sets6Panel.buttons[i]) {
                    pd.sendFloat("juce-sets6", static_cast<float>(i));
                    break;
                }
                else if (sets7Panel.buttons[i].getToggleState() && button == &sets7Panel.buttons[i]) {
                    pd.sendFloat("juce-sets7", static_cast<float>(i));
                    break;
                }
                else if (sets8Panel.buttons[i].getToggleState() && button == &sets8Panel.buttons[i]) {
                    pd.sendFloat("juce-sets8", static_cast<float>(i));
                    break;
                }
                else if (sets9Panel.buttons[i].getToggleState() && button == &sets9Panel.buttons[i]) {
                    pd.sendFloat("juce-sets9", static_cast<float>(i));
                    break;
                }
                else if (sets10Panel.buttons[i].getToggleState() && button == &sets10Panel.buttons[i]) {
                    pd.sendFloat("juce-sets10", static_cast<float>(i));
                    break;
                }
                else if (sets11Panel.buttons[i].getToggleState() && button == &sets11Panel.buttons[i]) {
                    pd.sendFloat("juce-sets11", static_cast<float>(i));
                    break;
                }
            }
        }
    }
    catch (const std::exception& e) {
        DBG("Error in buttonClicked: " << e.what());
    }
}

void MainComponent::sliderValueChanged(juce::Slider* slider)
{
    try {
        if (slider == &bpmSlider) {
            pd.sendFloat("juce-bpm", static_cast<float>(bpmSlider.getValue()));
        }
    }
    catch (const std::exception& e) {
        DBG("Error in sliderValueChanged: " << e.what());
    }
}

void MainComponent::print(const std::string& message)
{
    DBG("[Pd print] " + message);
}

// スレッドセーフなUI更新ヘルパーメソッド
void MainComponent::safeUpdateUI(const std::function<void()>& updateFunction)
{
    if (juce::MessageManager::getInstance()->isThisTheMessageThread()) {
        // すでにメインスレッドなら直接実行
        if (isValidState())
            updateFunction();
    }
    else {
        // 別スレッドからならメインスレッドにポスト
        juce::MessageManager::callAsync([this, updateFunction]() {
            if (isValidState())
                updateFunction();
        });
    }
}

void MainComponent::receiveFloat(const std::string& dest, float value)
{
    DBG("[Pd receiveFloat] from: " << dest << ", value: " << value);
    
    try {
        safeUpdateUI([this, dest, value]() {
            // トランスポートコントロール状態
            if (dest == "playbutton") {
                playButton.setToggleState(value > 0.0f, juce::dontSendNotification);
            }
            else if (dest == "setpause") {
                pauseButton.setToggleState(value > 0.0f, juce::dontSendNotification);
            }
            else if (dest == "setloop") {
                loopButton.setToggleState(value > 0.0f, juce::dontSendNotification);
            }
            else if (dest == "loadBPM") {
                bpmSlider.setValue(value, juce::dontSendNotification);
            }
            // 位置情報
            else if (dest == "countpos") {
                currentPosLabel.setText("Position: " + juce::String(value), juce::dontSendNotification);
            }
            else if (dest == "lastpos") {
                lastPosLabel.setText("Last Pos: " + juce::String(value), juce::dontSendNotification);
            }
            else if (dest == "alltime") {
                allTimeLabel.setText("Total Time: " + juce::String(value, 2) + " min", juce::dontSendNotification);
            }
            // チャプターセレクト
            else if (dest == "loadchapter") {
                int chapter = static_cast<int>(value);
                chapterBtn1.setToggleState(chapter == 0, juce::dontSendNotification);
                chapterBtn2.setToggleState(chapter == 1, juce::dontSendNotification);
                chapterBtn3.setToggleState(chapter == 2, juce::dontSendNotification);
                chapterBtn4.setToggleState(chapter == 3, juce::dontSendNotification);
            }
            // ビートバリエーション
            else if (dest == "loadbeat") {
                int beat = static_cast<int>(value);
                beatVariationBtn1.setToggleState(beat == 0, juce::dontSendNotification);
                beatVariationBtn2.setToggleState(beat == 1, juce::dontSendNotification);
            }
            // インバート状態
            else if (dest == "setbeatinvert") {
                beatInvertButton.setToggleState(value > 0.0f, juce::dontSendNotification);
            }
            else if (dest == "setarpeggioinvert") {
                arpeggioInvertButton.setToggleState(value > 0.0f, juce::dontSendNotification);
            }
            else if (dest == "setpaninvert") {
                panInvertButton.setToggleState(value > 0.0f, juce::dontSendNotification);
            }
            // ソロ状態
            else if (dest == "string-state") {
                stringSoloButton.setToggleState(value > 0.0f, juce::dontSendNotification);
            }
            else if (dest == "piano-state") {
                pianoSoloButton.setToggleState(value > 0.0f, juce::dontSendNotification);
            }
            // パラメーターセット状態
            else if (dest == "sets1") {
                int index = static_cast<int>(value);
                if (index >= 0 && index < 11) {
                    for (int i = 0; i < 11; ++i) {
                        sets1Panel.buttons[i].setToggleState(i == index, juce::dontSendNotification);
                    }
                    sets1Panel.selectedIndex = index;
                }
            }
            else if (dest == "sets2") {
                int index = static_cast<int>(value);
                if (index >= 0 && index < 11) {
                    for (int i = 0; i < 11; ++i) {
                        sets2Panel.buttons[i].setToggleState(i == index, juce::dontSendNotification);
                    }
                    sets2Panel.selectedIndex = index;
                }
            }
            else if (dest == "sets3") {
                int index = static_cast<int>(value);
                if (index >= 0 && index < 11) {
                    for (int i = 0; i < 11; ++i) {
                        sets3Panel.buttons[i].setToggleState(i == index, juce::dontSendNotification);
                    }
                    sets3Panel.selectedIndex = index;
                }
            }
            else if (dest == "sets4") {
                int index = static_cast<int>(value);
                if (index >= 0 && index < 11) {
                    for (int i = 0; i < 11; ++i) {
                        sets4Panel.buttons[i].setToggleState(i == index, juce::dontSendNotification);
                    }
                    sets4Panel.selectedIndex = index;
                }
            }
            else if (dest == "sets5") {
                int index = static_cast<int>(value);
                if (index >= 0 && index < 11) {
                    for (int i = 0; i < 11; ++i) {
                        sets5Panel.buttons[i].setToggleState(i == index, juce::dontSendNotification);
                    }
                    sets5Panel.selectedIndex = index;
                }
            }
            else if (dest == "sets6") {
                int index = static_cast<int>(value);
                if (index >= 0 && index < 11) {
                    for (int i = 0; i < 11; ++i) {
                        sets6Panel.buttons[i].setToggleState(i == index, juce::dontSendNotification);
                    }
                    sets6Panel.selectedIndex = index;
                }
            }
            else if (dest == "sets7") {
                int index = static_cast<int>(value);
                if (index >= 0 && index < 11) {
                    for (int i = 0; i < 11; ++i) {
                        sets7Panel.buttons[i].setToggleState(i == index, juce::dontSendNotification);
                    }
                    sets7Panel.selectedIndex = index;
                }
            }
            else if (dest == "sets8") {
                int index = static_cast<int>(value);
                if (index >= 0 && index < 11) {
                    for (int i = 0; i < 11; ++i) {
                        sets8Panel.buttons[i].setToggleState(i == index, juce::dontSendNotification);
                    }
                    sets8Panel.selectedIndex = index;
                }
            }
            else if (dest == "sets9") {
                int index = static_cast<int>(value);
                if (index >= 0 && index < 11) {
                    for (int i = 0; i < 11; ++i) {
                        sets9Panel.buttons[i].setToggleState(i == index, juce::dontSendNotification);
                    }
                    sets9Panel.selectedIndex = index;
                }
            }
            else if (dest == "sets10") {
                int index = static_cast<int>(value);
                if (index >= 0 && index < 11) {
                    for (int i = 0; i < 11; ++i) {
                        sets10Panel.buttons[i].setToggleState(i == index, juce::dontSendNotification);
                    }
                    sets10Panel.selectedIndex = index;
                }
            }
            else if (dest == "sets11") {
                int index = static_cast<int>(value);
                if (index >= 0 && index < 11) {
                    for (int i = 0; i < 11; ++i) {
                        sets11Panel.buttons[i].setToggleState(i == index, juce::dontSendNotification);
                    }
                    sets11Panel.selectedIndex = index;
                }
            }
        });
    }
    catch (const std::exception& e) {
        DBG("Error in receiveFloat: " << e.what());
    }
}

// MainComponent::receiveBang を修正して通知を受け取れるようにする
void MainComponent::receiveBang(const std::string& dest)
{
    DBG("[Pd receiveBang] from: " << dest);
    
    try {
        safeUpdateUI([this, dest]() {
            // 既存のコード...
            
            // PureDataからの操作完了通知
            if (dest == "pd-save-complete") {
                // 保存完了メッセージ表示
                DBG("Save operation completed by PureData");
                juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::InfoIcon,
                                                       "Save Complete",
                                                       "Settings have been saved successfully");
            }
            else if (dest == "pd-load-complete") {
                // 読み込み完了メッセージ表示
                DBG("Load operation completed by PureData");
                juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::InfoIcon,
                                                       "Load Complete",
                                                       "Settings have been loaded successfully");
            }
        });
    }
    catch (const std::exception& e) {
        DBG("Error in receiveBang: " << e.what());
    }
}


// ---------------------------------------------------------------------------
// MIDI の後処理（2026-10-25 追加）
// パッチが書き出す .mid には note-on しかなく、休符・音の終わりは、低い音の note-on
// （ノート 0、ピアノでは 0 に +12 したノート 12）で代用されている。
// ここで次の処理をする。
//  1. 次のイベントの時刻に note-off を足す。各音の長さは「次のイベントの時刻まで」で、
//     Logic の「後続のノートまでトリム」と同じ結果になる。
//  2. 休符の目印（kMinPlayableNote 未満）を消す。
//  3. ピアノだけ、piano-arpeggio のファイルが「分散和音（ノート 11）」を示す区間では、
//     和音の音を、音の長さの 1/4 ずつ、順に遅らせる（パッチの再生と同じ考え方）。
// 奏法・アルペジオのファイル（*-articulation, *-arpeggio）は、ノートナンバーが
// 情報そのものなので、対象外にする。
// ---------------------------------------------------------------------------
static constexpr int kMinPlayableNote = 21;  // ピアノの最低音 A0
static constexpr int kArpeggioOnNote  = 11;  // piano-arpeggio のノート：11＝分散和音、10＝同時

// piano-arpeggio の .mid から、分散和音のオン・オフの切り替え（時刻, ノート）を読む
static std::vector<std::pair<double, int>> readArpeggioStates (const juce::File& arpeggioFile)
{
    std::vector<std::pair<double, int>> states;
    juce::MidiFile midi;
    juce::FileInputStream in (arpeggioFile);
    if (! in.openedOk() || ! midi.readFrom (in, false) || midi.getNumTracks() < 1)
        return states;

    const auto* track = midi.getTrack (0);
    for (int i = 0; i < track->getNumEvents(); ++i)
    {
        const auto& m = track->getEventPointer (i)->message;
        if (m.isNoteOn())
            states.emplace_back (m.getTimeStamp(), m.getNoteNumber());
    }
    std::stable_sort (states.begin(), states.end(),
                      [] (const auto& x, const auto& y) { return x.first < y.first; });
    return states;
}

static bool isArpeggioAt (const std::vector<std::pair<double, int>>& states, double time)
{
    int note = 10; // 初期値：同時
    for (const auto& st : states)
    {
        if (st.first <= time) note = st.second;
        else break;
    }
    return note == kArpeggioOnNote;
}

// arpeggioStates が nullptr でなければ、分散和音を反映する（ピアノ用）
static bool addNoteOffsToMidiFile (const juce::File& file,
                                   const std::vector<std::pair<double, int>>* arpeggioStates)
{
    juce::MidiFile midi;
    {
        juce::FileInputStream in (file);
        if (! in.openedOk() || ! midi.readFrom (in, false)) // false: 読み込み時に note-off を自動追加しない
            return false;
    }

    const int ticksPerQuarter = midi.getTimeFormat();
    if (ticksPerQuarter <= 0)
        return false; // SMPTE 形式は対象外

    juce::MidiFile outFile;
    outFile.setTicksPerQuarterNote (ticksPerQuarter);

    for (int trackIndex = 0; trackIndex < midi.getNumTracks(); ++trackIndex)
    {
        const auto* track = midi.getTrack (trackIndex);
        juce::MidiMessageSequence outSeq;

        // note-on 以外（コントロールチェンジ、プログラムチェンジ、メタイベント）はそのまま残す。
        // 時刻ごとに note-on をまとめる。
        std::map<double, std::vector<juce::MidiMessage>> noteOnsByTime;
        for (int i = 0; i < track->getNumEvents(); ++i)
        {
            const auto& m = track->getEventPointer (i)->message;
            if (m.isNoteOn()) // 速度 0 の note-on は、ここでは note-on とみなさない
                noteOnsByTime[m.getTimeStamp()].push_back (m);
            else if (! m.isEndOfTrackMetaEvent())
                outSeq.addEvent (m);
        }

        std::vector<double> times;
        for (const auto& kv : noteOnsByTime)
            times.push_back (kv.first);

        // 最後の音は、次のイベントがないので、直前の間隔と同じ長さにする
        const double lastGap = times.size() >= 2 ? juce::jmax (1.0, times.back() - times[times.size() - 2]) : 1.0;

        std::vector<juce::MidiMessage> sounding; // 今鳴っている音（次の時刻で note-off にする）
        for (size_t gi = 0; gi < times.size(); ++gi)
        {
            const double time = times[gi];
            const double nextTime = (gi + 1 < times.size()) ? times[gi + 1] : time + lastGap;
            const double length = nextTime - time;

            // 前の時刻から鳴っていた音を止める（note-off を、同じ時刻の note-on より先に入れる）
            for (const auto& on : sounding)
                outSeq.addEvent (juce::MidiMessage::noteOff (on.getChannel(), on.getNoteNumber(), (juce::uint8) 0), time);
            sounding.clear();

            const bool arpeggio = arpeggioStates != nullptr && isArpeggioAt (*arpeggioStates, time);

            // 休符の目印（ノート 0、ノート 12 など）は書き出さない。
            // 分散和音では、残った音を、ファイルに書かれている順に、音の長さの 1/4 ずつ遅らせる。
            int k = 0;
            for (const auto& on : noteOnsByTime[time])
            {
                if (on.getNoteNumber() < kMinPlayableNote)
                    continue;

                auto note = on;
                if (arpeggio)
                    note.setTimeStamp (time + std::round (k * length / 4.0));
                outSeq.addEvent (note);
                sounding.push_back (note);
                ++k;
            }
        }

        // 最後のグループの音を止める
        if (! times.empty())
            for (const auto& on : sounding)
                outSeq.addEvent (juce::MidiMessage::noteOff (on.getChannel(), on.getNoteNumber(), (juce::uint8) 0),
                                 times.back() + lastGap);

        outSeq.sort(); // 同じ時刻では、先に入れた順（note-off → note-on）が保たれる
        outSeq.updateMatchedPairs();
        outFile.addTrack (outSeq);
    }

    // 上書き（途中で失敗してもファイルを壊さないよう、一時ファイルに書いてから置き換える）
    juce::File tmp = file.getSiblingFile (file.getFileName() + ".tmp");
    {
        juce::FileOutputStream out (tmp);
        if (! out.openedOk())
            return false;
        out.setPosition (0);
        out.truncate();
        if (! outFile.writeTo (out, 1))
            return false;
    }
    return tmp.moveFileTo (file);
}

static void addNoteOffsToInstrumentMidiFiles (const juce::File& folder)
{
    // cello_…, viola_…, violin1_…, violin2_…, piano_… の .mid だけが対象
    for (const auto& f : folder.findChildFiles (juce::File::findFiles, false, "*.mid"))
    {
        const auto name = f.getFileName();
        const bool isString = name.startsWith ("cello_") || name.startsWith ("viola_")
                           || name.startsWith ("violin1_") || name.startsWith ("violin2_");
        const bool isPiano = name.startsWith ("piano_");
        if (! isString && ! isPiano)
            continue;

        bool ok = false;
        if (isPiano)
        {
            // piano_<日時>.mid に対応する piano-arpeggio_<日時>.mid
            const auto arpeggioFile = f.getSiblingFile ("piano-arpeggio_" + name.fromFirstOccurrenceOf ("piano_", false, false));
            std::vector<std::pair<double, int>> states;
            if (arpeggioFile.existsAsFile())
                states = readArpeggioStates (arpeggioFile);
            ok = addNoteOffsToMidiFile (f, states.empty() ? nullptr : &states);
        }
        else
        {
            ok = addNoteOffsToMidiFile (f, nullptr);
        }

        if (! ok)
            DBG ("Could not add note-offs to: " + f.getFullPathName());
    }
}

// MainComponent.cpp に追加するファイル操作関連のコード（エラー修正版）
void MainComponent::saveTextFile()
{
    // saveディレクトリの確認と作成
    juce::File saveDir = defaultDirectory.getChildFile("save");
    juce::File presetsDir = defaultDirectory.getChildFile("presets");
    
    if (!saveDir.exists())
        saveDir.createDirectory();
    if (!presetsDir.exists())
        presetsDir.createDirectory();
    
    // 現在の日時を取得してデフォルトのファイル名を生成
    juce::Time now = juce::Time::getCurrentTime();
    juce::String timeStamp = now.formatted("%Y%m%d_%H%M%S");
    juce::String defaultSaveName = "Save_" + timeStamp + ".txt"; // save用
    juce::String defaultPresetName = "Preset_" + timeStamp + ".txt"; // presets用
    
    // まずpresets用の保存ダイアログを表示
    std::shared_ptr<juce::FileChooser> presetChooser = std::make_shared<juce::FileChooser>(
                                                                                           "Save Preset As...",
                                                                                           presetsDir.getChildFile(defaultPresetName), // デフォルトの保存先とファイル名
                                                                                           "*.txt", // 拡張子フィルター
                                                                                           true);   // ネイティブダイアログを使用
    
    // 非同期で保存ダイアログを表示（プリセット用）
    auto presetFlags = juce::FileBrowserComponent::saveMode |
    juce::FileBrowserComponent::canSelectFiles |
    juce::FileBrowserComponent::warnAboutOverwriting;
    
    presetChooser->launchAsync(presetFlags, [this, presetChooser, saveDir, defaultSaveName, timeStamp](const juce::FileChooser& fc) {
        if (fc.getResults().size() > 0)
        {
            juce::File selectedPresetFile = fc.getResult();
            
            // プリセットを保存
            pd.sendSymbol("juce-preset-path", selectedPresetFile.getFullPathName().toStdString());
            pd.sendBang("juce-do-presets-save");
            
            // デバッグメッセージ
            DBG("Requested PureData to save preset in: " + selectedPresetFile.getFullPathName());
            
            // 次に通常のsaveファイル保存ダイアログを表示
            juce::Timer::callAfterDelay(500, [this, saveDir, defaultSaveName, timeStamp]() {
                // 保存用のFileChooserを作成
                std::shared_ptr<juce::FileChooser> saveChooser = std::make_shared<juce::FileChooser>(
                                                                                                     "Save Data As...",
                                                                                                     saveDir.getChildFile(defaultSaveName), // デフォルトの保存先とファイル名
                                                                                                     "*.txt", // 拡張子フィルター
                                                                                                     true);   // ネイティブダイアログを使用
                
                // 非同期で保存ダイアログを表示（データ用）
                auto saveFlags = juce::FileBrowserComponent::saveMode |
                juce::FileBrowserComponent::canSelectFiles |
                juce::FileBrowserComponent::warnAboutOverwriting;
                
                saveChooser->launchAsync(saveFlags, [this, saveChooser, saveDir](const juce::FileChooser& fc) {
                    if (fc.getResults().size() > 0)
                    {
                        juce::File selectedSaveFile = fc.getResult();
                        juce::String dirName = selectedSaveFile.getFileNameWithoutExtension();
                        juce::File saveFolder = saveDir.getChildFile(dirName);
                        
                        // サブディレクトリを作成
                        if (!saveFolder.exists())
                        {
                            auto result = saveFolder.createDirectory();
                            if (result.failed())
                            {
                                DBG("Failed to create save directory: " +
                                    saveFolder.getFullPathName() + " Error: " + result.getErrorMessage());
                                
                                juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                                                                       "Save Error",
                                                                       "Could not create save directory.");
                                return;
                            }
                        }
                        
                        // ユーザー指定の名前のディレクトリに対する保存処理
                        pd.sendSymbol("juce-save-path", saveFolder.getFullPathName().toStdString());
                        pd.sendBang("juce-do-save-data");
                        
                        DBG("Requested PureData to save data in: " + saveFolder.getFullPathName());
                        
                        // 保存完了メッセージを表示
                        juce::Timer::callAfterDelay(1000, [dirName, saveFolder]() {
                            // パッチが書き出した .mid に、note-off を足す（休符の目印は消す）
                            addNoteOffsToInstrumentMidiFiles (saveFolder);

                            juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::InfoIcon,
                                                                   "Save Complete",
                                                                   "Data saved as: " + dirName);
                        });
                    }
                });
            });
        }
    });
}

// 読み込み処理 - loadボタンを押した時に実行される
void MainComponent::loadTextFile()
{
    // ディレクトリ確認
    juce::File presetsDir = defaultDirectory.getChildFile("presets");
    if (!presetsDir.exists())
        presetsDir.createDirectory();
    
    DBG("Using presets directory: " + presetsDir.getFullPathName());
    
    std::shared_ptr<juce::FileChooser> chooser = std::make_shared<juce::FileChooser>(
                                                                                     "Load Preset", presetsDir, "*.txt");
    
    // 非同期でダイアログを表示
    chooser->launchAsync(juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
                         [this, chooser](const juce::FileChooser& fc) {
        auto selectedFile = fc.getResult();
        
        if (selectedFile.existsAsFile()) {
            // 選択したファイルのパスをPdに送信
            DBG("Selected file: " + selectedFile.getFullPathName());
            pd.sendSymbol("juce-file-path", selectedFile.getFullPathName().toStdString());
            
            // ロードコマンドを送信
            juce::Timer::callAfterDelay(100, [this]() {
                pd.sendBang("juce-do-load");
                DBG("Sent load command to PD");
            });
        }
    }
                         );
}
