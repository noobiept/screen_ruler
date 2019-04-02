try:
    from source.ruler import main

except ImportError:
    from ruler import main


def go():
    main.run()


if __name__ == '__main__':
    go()
