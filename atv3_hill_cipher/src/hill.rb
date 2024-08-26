require 'matrix'

class Hill
  def self.encrypt(plain_text, key)
    assert_matrix(key)
    raise ArgumentError, "text must be ASCII only" unless plain_text.ascii_only?

    order = key.row_size
    padded_text = pad(plain_text, order)
    blocks = padded_text.chars.each_slice(order).map { |block| block.map(&:ord) }
    cipher_blocks = blocks.map { |block| (key * Vector[*block]).to_a }
    cipher_blocks.flatten.map { |el| (el % 128).chr }.join
  end

  def self.decrypt(cipher_text, key)
    assert_matrix(key)
    raise ArgumentError, "text must be ASCII only" unless cipher_text.ascii_only?

    key = matrix_mod_inverse(key, 128)
    encrypt(cipher_text, key)
  end

  def self.key_from_file(file_path)
    lines = File.readlines(file_path).map(&:chomp)
    rows = lines.map { |line| line.split.map(&:to_i) }
    Matrix[*rows]
  end

  private

  def self.assert_matrix(matrix)
    raise ArgumentError, "key matrix must be square" unless matrix.square?
    raise ArgumentError, "key matrix must be invertible" unless matrix.regular?
    raise ArgumentError, "key matrix must have determinant coprime with 128" unless matrix.det.gcd(128) == 1
  end

  def self.pad(str, order)
    padding = order - (str.length % order)
    str += "\0" * padding
  end

  def self.mod_inverse(a, m)
    m.times do |i|
      return i if (a * i) % m == 1
    end
  end

  def self.matrix_mod_inverse(matrix, mod)
    det = matrix.determinant
    inv_det = mod_inverse(det, mod)
    adjugate = matrix.adjugate.map { |el| el % mod }

    inverse_matrix = Matrix.build(matrix.row_count) do |row, col|
      (adjugate[row, col] * inv_det) % mod
    end

    inverse_matrix
  end
end
