#include "Miscellaneous.h"
#include "raymath.h"


TimedText::TimedText(const std::function<void()> drawFunction, Reason reason)
    : drawFunction(drawFunction), lastCheckTime(0.0f), duration(3.0f), text(""), reason(reason)
{
    lastCheckTime = GetTime();
};

void TimedText::Draw()
{
    drawFunction();
}


TimedText* Utilities::GetTimedText(char const* text, Reason reason)
{
    Color color = YELLOW;

    if (reason == Reason::Smash) {
        color = GREEN;
    } else if (reason == Reason::Dizzyness) {
        color = ORANGE;
    } else if (reason == Reason::FaceHit) {
        color = PURPLE;
    } else if (reason == Reason::AlmostGameOver) {
        color = DARKPURPLE;
    }

    int fontSize = 30;
    float x = (GetScreenWidth() - MeasureText(text, fontSize)) / 2;
    float y = GetScreenHeight() / 3;
    TimedText* timedText = new TimedText([fontSize, color, text, x, y](){
        DrawText(text, x, y, fontSize, color);
    }, reason);
    timedText->lastCheckTime = GetTime();
    return timedText;
}

Vector3 Utilities::AbsVector3(Vector3& vector3)
{
    return Vector3({(float)abs(vector3.x), (float)abs(vector3.y), (float)abs(vector3.z)});
}

Vector3 Utilities::QuantizeVector3(Vector3& vector3)
{
    Vector3 copyVector3({vector3.x, vector3.y, vector3.z});
    copyVector3.x = round(copyVector3.x * 2.0f) / 2.0f;
    copyVector3.y = round(copyVector3.y * 2.0f) / 2.0f;
    copyVector3.z = round(copyVector3.z * 2.0f) / 2.0f;
    return copyVector3;
}

int Utilities::SumVector3(Vector3& vector3)
{
    return (int)(vector3.x + vector3.y + vector3.z);
}

void Utilities::Log(std::string message, std::string prefix, TraceLogLevel logLevel)
{
    TraceLog(logLevel, ("[" + prefix + "]: " + message).c_str());
}

const char* Utilities::GetQuote(Reason reason)
{
    std::vector<const char*> onFaceHitQuotes = {
        "My face! No more Mr. Nice Cube!",
        "Oof! That's gonna leave a dent... in the pride!",
        "Cube abuse! I'm callin' a lawyer!",
        "WHO PUT THE GROUND THERE?!",
        "I swear, the floor moved first.",
        "I was born to roll, not to suffer!",
        "Great, now my handsome face has a scratch.",
        "You mess with the cube, you get the bruise!",
        "This is why I have trust issues with gravity!",
        "Anger... rising... must... crush... everything!",
        "Face-first into failure... again!",
        "Why do I even HAVE a face?!",
        "The floor and I have issues.",
        "Ugh! That's the good cheek!",
        "One more hit and I'm filing a complaint with geometry.",
        "I roll with fury, but land with regret.",
        "Note to self: install shock absorbers.",
        "That's not how I wanted to “make an impression!”",
        "If I had feelings, they'd be hurt.",
        "You win this round, gravity... but I'll be back."
    };
    std::vector<const char*> onDizzynessQuotes = {
        "Why is the world spinning?! Oh wait—IT'S ME!",
        "I swear I saw three of that triangle... and I hate all of them!",
        "Someone stop the ride! I wanna SMASH something!",
        "Wobble mode: activated. Rage level: MAXED.",
        "Everything is a blur... except my fury!",
        "I'm not dizzy... I'm just... tactically unstable!",
        "Spinning was a mistake. Violence is the cure!",
        "My edges! My beautiful, perfect edges!",
        "This isn't disorientation—it's recalibrated rage!",
        "Even my anger is dizzy. That's how bad it is.",
        "Who tilted the universe?!",
        "AAAAAAAAAAARGH!",
        "I'm one roll away from losing what's left of my sanity.",
        "Forget the icosahedron—gravity is my real enemy.",
        "I've got a headache in all six directions!",
        "THIS is why cubes weren't meant to spin like Beyblades!",
        "I'm gonna hurl... and then smash whoever made me hurl!",
        "If I spin any more, I'm turning into a sphere.",
        "I can't see straight but I can still CRUSH straight!",
        "My corners are confused and my face is furious!",
        "You spin me right round, baby... INTO A RAMPAGE!"
    };

    std::vector<const char*> onSmashQuotes = {
        "Cube smash!",
        "SMASHED IT! Like a boss!",
        "You just got CUBED!",
        "One down, infinity to go!",
        "Hope you had insurance!",
        "Feel the wrath of my corners!",
        "Didn't like you anyway.",
        "Next time, try not existing near me!",
        "Ooooh, that felt good. Wanna go again?!",
        "Flattened like yesterday's polygons!",
        "That's for being in my way.",
        "You make a lovely stain!",
        "Crushed dreams. Crushed bones. I'm on fire!",
        "Another victim of righteous geometry!",
        "That was... oddly satisfying.",
        "Cleanup on aisle: YOU.",
        "Too slow, too small, too squished!",
        "Crush goals = achieved.",
        "Nice flock. Shame if someone SMASHED IT.",
        "Boom! Face meets floor meets YOU!",
        "Wanna get smashed? Line up!",
    };

    std::vector<const char*> onAlmostGameOverQuotes = {
        "TOO... MUCH... RAAAAAGE!",
        "I CAN'T BE CUBED ANY LONGER!!!",
        "I'M A DANGER TO EVERYTHING!",
        "THE FLOOR. THE SKY. THE AIR. IT'S ALL WRONG!",
        "DON'T TOUCH ME OR I'LL SHATTER THE UNIVERSE!",
        "I HAVE NO CHILL LEFT!",
        "ONE MORE HIT AND I'M A SQUARE GONE SUPERNOVA!",
        "THE NEXT THING I SEE... GETS DESTROYED.",
        "I'M ROLLING ON RAGE FUMES!!",
        "EVEN I'M SCARED OF ME RIGHT NOW.",
        "THE WORLD'S NOT READY FOR CUBE-AGEDDON!",
        "EVERY ANGLE IS THE WRONG ANGLE!",
        "MY FACE HURTS. MY SOUL HURTS. YOU'RE NEXT.",
        "IF I STOP ROLLING, I EXPLODE.",
        "I'M A GEOMETRIC NIGHTMARE!",
        "MY BLOOD TYPE IS PURE HATRED!",
        "CUBES WEREN'T MEANT TO FEEL THIS MUCH!",
        "WHY AM I SHOUTING?! OH YEAH—WRATH MODE!!",
        "EVEN MY INNER MONOLOGUE IS SCREAMING!",
        "GOODBYE SANITY. HELLO ANNIHILATION."
    };

    if (reason == Reason::FaceHit)
    {
        int choice = GetRandomValue(0, onFaceHitQuotes.size() - 1);
        return onFaceHitQuotes.at(choice);
    } else if (reason == Reason::Dizzyness)
    {
        int choice = GetRandomValue(0, onDizzynessQuotes.size() - 1);
        return onDizzynessQuotes.at(choice);
    } else if (reason == Reason::Smash)
    {
        int choice = GetRandomValue(0, onSmashQuotes.size() - 1);
        return onSmashQuotes.at(choice);
    } else if (reason == Reason::AlmostGameOver) {
        int choice = GetRandomValue(0, onAlmostGameOverQuotes.size() - 1);
        return onAlmostGameOverQuotes.at(choice);
    }
    return "";
}
