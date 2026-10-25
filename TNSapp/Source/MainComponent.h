#pragma once

#include <JuceHeader.h>
#include "PdBase.hpp"
#include "PdTypes.hpp"
#include "ParameterLookAndFeel.h"

class MainComponent : public juce::AudioAppComponent,
public juce::Button::Listener,
public juce::Slider::Listener,
public pd::PdReceiver

{
    public:
    MainComponent();
    ~MainComponent() override;
    
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    
    void paint(juce::Graphics& g) override;
    void resized() override;
    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;
    
    // Pdメッセージ受信
    void print(const std::string& message) override;
    void receiveFloat(const std::string& dest, float value) override;
    void receiveBang(const std::string& dest) override;
    
    // スレッドセーフなUI更新
    void safeUpdateUI(const std::function<void()>& updateFunction);
    bool isValidState() const { return isShowing(); }
    
    // プログレスバー更新用メソッド
    void updateProgressBar();
    
    private:
    pd::PdBase pd;
    
    // カスタムLookAndFeel
    ParameterLookAndFeel parameterLookAndFeel;
    
    // 現在位置と最終位置の保持用変数
    float currentPosition = 0.0f;
    float lastPosition = 0.0f;
    float totalTime = 0.0f;
    
    void setupComponentColors();
    
    // トランスポートコントロール
    juce::ToggleButton playButton { "Play/Stop" };
    juce::ToggleButton pauseButton { "Pause" };
    juce::ToggleButton loopButton { "Loop" };
    
    // ファイル操作ボタン
    juce::TextButton loadButton { "Load File" };
    juce::TextButton saveButton { "Save" };
    juce::TextButton defaultButton { "Default" };
    juce::TextButton compileButton { "Compile" };
    
    // BPM調整スライダー
    juce::Label bpmLabel { "", "BPM:" };
    juce::Slider bpmSlider;
    
    // 基本的なファイル操作用のメンバ関数
    void saveTextFile();
    void loadTextFile();
    
    // ファイルパスの保存用
    juce::File defaultDirectory;
    juce::File currentTextFile;
    
    // プログレスバー
    class CustomProgressBar : public juce::Component
    {
        public:
        CustomProgressBar() : progress(0.0) {}
        
        void setProgress(double newProgress)
        {
            progress = juce::jlimit(0.0, 1.0, newProgress);
            repaint();
        }
        
        void setTextToDisplay(const juce::String& text)
        {
            displayText = text;
            repaint();
        }
        
        void setTextToShowPercentage(bool shouldShow)
        {
            showPercentage = shouldShow;
            repaint();
        }
        
        void paint(juce::Graphics& g) override
        {
            auto bounds = getLocalBounds().toFloat().reduced(1.0f);
            
            // 背景
            g.setColour(juce::Colours::darkgrey.withAlpha(0.3f));
            g.fillRoundedRectangle(bounds, 3.0f);
            
            // プログレスバー
            if (progress > 0.0)
            {
                auto progressWidth = bounds.getWidth() * progress;
                g.setColour(juce::Colours::orange);
                g.fillRoundedRectangle(bounds.withWidth(progressWidth), 3.0f);
            }
            
            // テキスト
            g.setColour(juce::Colours::white);
            g.setFont(juce::Font(14.0f));
            
            if (displayText.isNotEmpty())
                g.drawText(displayText, bounds, juce::Justification::centred);
            else if (showPercentage)
                g.drawText(juce::String(int(progress * 100)) + "%", bounds, juce::Justification::centred);
        }
        
        private:
        double progress;
        bool showPercentage = false;
        juce::String displayText;
    };
    
    CustomProgressBar progressBar;
    
    // 表示用ラベル
    juce::Label currentPosLabel { "", "Position: ---" };
    juce::Label lastPosLabel { "", "Last Pos: ---" };
    juce::Label allTimeLabel { "", "Total Time: ---" };
    
    // チャプターセレクト
    juce::Label chapterSelectLabel { "", "Chapter Select:" };
    juce::ToggleButton chapterBtn1 { juce::CharPointer_UTF8("第1章") };
    juce::ToggleButton chapterBtn2 { juce::CharPointer_UTF8("第3章") };
    juce::ToggleButton chapterBtn3 { juce::CharPointer_UTF8("第10章") };
    juce::ToggleButton chapterBtn4 { juce::CharPointer_UTF8("結") };
    
    // ビートバリエーション
    juce::Label beatVariationLabel { "", "Beat Variation:" };
    juce::ToggleButton beatVariationBtn1 { "Simple" };
    juce::ToggleButton beatVariationBtn2 { "Complex" };
    
    // インバートコントロール
    juce::ToggleButton beatInvertButton { "Beat-invert" };
    juce::ToggleButton arpeggioInvertButton { "Arpeggio-invert" };
    juce::ToggleButton panInvertButton { "Pan-invert" };
    
    // ソロコントロール
    juce::ToggleButton stringSoloButton { "String" };
    juce::ToggleButton pianoSoloButton { "Piano" };
    
    // 垂直ラジオボタン（test7.pd専用）
    juce::Label paramSetsLabel { "", "Parameter Sets:" };
    // 11グループのラジオボタンパネル
    struct RadioButtonPanel
    {
        juce::String name;
        juce::Label titleLabel;
        juce::TextButton resetButton;
        juce::ToggleButton buttons[11];
        int selectedIndex;
        
        RadioButtonPanel(const juce::String& labelText)
        : name(labelText),
        titleLabel("", labelText),
        resetButton("R"),
        selectedIndex(0)
        {
            for (int i = 0; i < 11; ++i)
                buttons[i].setToggleState(false, juce::dontSendNotification);
            
            // 最初のボタンは選択状態に
            if (buttons[0].isVisible())
                buttons[0].setToggleState(true, juce::dontSendNotification);
        }
    };
    
    // 11種類のパラメーターセット
    RadioButtonPanel sets1Panel { juce::CharPointer_UTF8("Octave") };
    RadioButtonPanel sets2Panel { juce::CharPointer_UTF8("Permutation") };
    RadioButtonPanel sets3Panel { juce::CharPointer_UTF8("Chord") };
    RadioButtonPanel sets4Panel { juce::CharPointer_UTF8("Key-1") };
    RadioButtonPanel sets5Panel { juce::CharPointer_UTF8("Key-2") };
    RadioButtonPanel sets6Panel { juce::CharPointer_UTF8("Key-3") };
    RadioButtonPanel sets7Panel { juce::CharPointer_UTF8("Pan-1") };
    RadioButtonPanel sets8Panel { juce::CharPointer_UTF8("Pan-2") };
    RadioButtonPanel sets9Panel { juce::CharPointer_UTF8("Velocity") };
    RadioButtonPanel sets10Panel { juce::CharPointer_UTF8("Beat") };
    RadioButtonPanel sets11Panel { juce::CharPointer_UTF8("Articulation") };
    
    // パラメーター名ラベル（左側）
    struct ParamLabelSet
    {
        juce::Label labels[11];
        
        ParamLabelSet()
        {
            const juce::String labelTexts[] = {
                juce::CharPointer_UTF8("penspeed"),
                juce::CharPointer_UTF8("angle"),
                juce::CharPointer_UTF8("penblur"),
                juce::CharPointer_UTF8("erosion"),
                juce::CharPointer_UTF8("penslight"),
                juce::CharPointer_UTF8("coefficient"),
                juce::CharPointer_UTF8("width"),
                juce::CharPointer_UTF8("height"),
                juce::CharPointer_UTF8("speed"),
                juce::CharPointer_UTF8("depth"),
                juce::CharPointer_UTF8("baseline")
            };
            
            for (int i = 0; i < 11; ++i)
            {
                labels[i].setText(labelTexts[i], juce::dontSendNotification);
                labels[i].setJustificationType(juce::Justification::right);
            }
        }
    };
    
    ParamLabelSet paramLabels;
    juce::Viewport viewport;
    juce::Component* parametersComponent = nullptr;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
