#!/usr/bin/env python

import socket
import readchar

if __name__ == "__main__":
    # Set TCP Information
    host = '10.155.34.47'
    port = 8480

    # Connect to TCP Server
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # Wait to be connected
    connected = False
    while not connected:
        try:
            print("Attempting to Connect")
            s.connect((host,port))
            connected = True
            print("Connected to TCP Server")
        except Exception as e:
            pass
            # print("Couldn't connect to TCP Server")
            # exit()

    # Send commands using StdIn
    quit = False;
    while not quit:
        match readchar.readchar():
            # Rotate Left
            case "h":
                print("Command Sent: TurnL")
                s.send("^TurnL".encode())

            # Move Backwards
            case "j":
                print("Command Sent: MoveB")
                s.send("^MoveB".encode())

            # Rotate Right
            case "k":
                print("Command Sent: MoveF")
                s.send("^MoveF".encode())

            # Rotate Left
            case "l":
                print("Command Sent: TurnR")
                s.send("^TurnR".encode())

            # Stop
            case " ":
                print("Command Sent: Stop")
                s.send("^Stop".encode())

            # Send Circle Command
            case "c":
                print("Command Sent: Circle")
                s.send("^Circle".encode())

            # Send Exit Command
            case "e":
                print("Command Sent: Exit")
                s.send("^Exit".encode())

            # Send Inc Command
            case "i":
                print("Command Sent: Exit")
                s.send("^Inc".encode())

            # Quit
            case "q":
                quit = True
                break

    # Close connection
    s.close()
