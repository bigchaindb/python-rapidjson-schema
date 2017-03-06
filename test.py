import rapidjson_schema

schema = """
{
	"title": "Person",
	"type": "object",
	"properties": {
		"firstName": {
			"type": "integer"
		},
		"lastName": {
			"type": "string"
		},
		"age": {
			"description": "Age in years",
			"type": "integer",
			"minimum": 0
		}
	},
	"required": ["firstName"]
}
"""

s = rapidjson_schema.loads(schema)
print(s.validate('{"firstName":"a","lastName":"b"}'))
print(s.validate('{}'))

