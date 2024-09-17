# [M1 QDCS] High performance computing

This is an introductory course on the basics of high performance and parallel computing, with a particular focus on multicore and vector parallelism on a single machine. Apart from parallelism, many multicore architecture details and various code optimization techniques will be covered throughout the course.

# Course organization

We will have seven sessions of 3 hours each (plus mini-exam, c.f. below). The first session is an introduction to high performance computing. The rest will be a mix of course (around 1h-1h30) and lab session (around 1h30-2h) on that subject. You might not be able to finish all exercises during the lab session (which is completely normal); you should make sure that you can understand and finish all basic exercises (first two or three) during the session so that you feel more confident to finish the rest on your own.

# Evaluation

## Graded lab assignments
Each lab assignment should be sent to the address `oguz.kaya[at]universite-paris-saclay.fr` with the subject format **"[M1QDCS]HPC LABX SURNAME(s) Name(s) LAB?"** (e.g., **[M1QDCS]HPC LAB3 ARNAULT-BARRAT Jean-François**) by 23:59 Sunday following the lab session. Solutions will be posted on the git repository just after this deadline; therefore, do not submit late!. Please follow this format for the email subject **to the letter** as I will employ filters to sort these submissions.

Please only attach source files (*.cpp) to your email, one file per exercise. **Do not zip** the files!

Out of all lab assignments, two of them will be selected randomly to be graded, each providing up to 2/20 points.

## End-of-session mini-exams
At the end of weeks 3, 4, 5, 6, there will be mini-exams for 4/20 points each. Three types of exercises that might appear on these mini-exams are

You can bring four A4 sheets to the mini-exam (both sides); no other material is allowed.

**Debugging:** Given a complete code with multiple bugs, the goal is to identify each bug (without correcting them), say what the problem is, and specify the corresponding line(s) in the code.

**Output:** Given a complete code, the goal is to find what the program could potentially display. There might be multiple possibilities, in which case you should indicate all of them.

**Coding:** A squeleton code or a function signature will be given for a specific task, and you will be expected to provide a parallel/optimized code that performs the intended task.

# Technical tips

Text editor: If you do not have a "preferred" text editor for development already, I recommend Visual Studio Code: https://code.visualstudio.com . You can follow the official tutorial for the basics: https://www.youtube.com/watch?v=B-s71n0dHUk&list=PLj6YeMhvp2S5UgiQnBfvD7XgOMKs3O_G6

You will need a GCC compiler for this course, aan x86-based machine for the vectorization part of the course. If you are using Linux on an Intel/AMD machine, you have all you need. 

On Windows, you will need to install Ubuntu using WSL: https://www.youtube.com/watch?v=wjbbl0TTMeo

On MacOS, you will need to first install Homebrew: https://www.youtube.com/watch?v=IWJKRmFLn-g

Then install GCC using Homebrew with the command: **brew install gcc**. Note that to compile using GCC on MacOS, you should use the command **g++-14** instead of **g++** (which defaults to the clang compiler that does not come with OpenMP).
