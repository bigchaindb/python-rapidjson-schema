from contextlib import contextmanager

import rapidjson_schema
from pytest import fixture, raises


schema_text = """
{
	"title": "Person",
	"type": "object",
	"properties": {
		"firstName": {
			"type": "string"
		},
		"age": {
                    "type": "integer"
		}
	},
	"required": ["firstName"]
}
"""


@fixture
def schema():
    return rapidjson_schema.loads(schema_text)


def test_init_invalid_schema():
    with raises(ValueError):
        rapidjson_schema.loads('')


def test_schema_pass(schema):
    schema.validate('{"firstName":"a","age":1}')


def test_schema_fail(schema):
    with raises(ValueError):
        schema.validate('{}')


def test_schema_type_error(schema):
    with raises(TypeError):
        schema.validate(1)
