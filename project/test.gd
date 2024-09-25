extends Node


@onready var serializer: GVTextureSerializer = GVTextureSerializer.new()
@onready var image_texture: CompressedTexture2D = load("res://icon.svg")

func _ready() -> void:
	# pass	
	var bytes:PackedByteArray = serializer.serializeCompressedTexture2D(image_texture)
	print(bytes.size())


func _process(_delta: float) -> void:
	pass
