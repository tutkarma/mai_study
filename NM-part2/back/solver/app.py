from flask import Flask

from solver.handlers.labs import labs


def create_app():
    app = Flask(__name__)
    app.register_blueprint(labs, url_prefix='/labs')
    return app
