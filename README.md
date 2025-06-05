# Esports-Management-System
Esports Management System for University.

This program simulates a single-elimination esports tournament using C++. It reads player data from a CSV file, groups them into teams, and schedules matches across multiple rounds until a champion is determined.

Features:
Loads 200 players from a CSV file

Forms 20 teams (10 players each)

Computes team rankings based on average player skill

Simulates match rounds using a custom circular queue

Displays round-by-round results and final champion

Avoids STL containers by using static arrays and function pointers

Structure:
CSVReader.cpp/.hpp – Reads and parses player data

Teams.hpp – Defines team structure and ranking logic

TeamScheduler.cpp/.hpp – Manages tournament progression and match outcomes

MatchQueue.hpp – Implements circular queue for match scheduling

main.cpp – Ties everything together and handles output
